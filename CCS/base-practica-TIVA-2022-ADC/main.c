#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "utils/uartstdio.h"
#include "drivers/buttons.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "utils/cpu_usage.h"

#include "drivers/rgb.h"
#include "drivers/configADC.h"
#include "commands.h"

#include <remotelink.h>
#include <serialprotocol.h>

// Fichero creado para declarar las etiquetas el manejador de eventos
#include "drivers/ColaEventos.h"
#include "drivers/ACME_1623.h"
#include "drivers/BMI160.h"

//parametros de funcionamiento de la tareas
#define REMOTELINK_TASK_STACK (512)
#define REMOTELINK_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define COMMAND_TASK_STACK (512)
#define COMMAND_TASK_PRIORITY (tskIDLE_PRIORITY+1)

// Prioridad para la tarea maestra
#define MASTERTASKPRIO 1
// Tamaño de pila para la tarea maestra
#define MASTERTASKSIZE 512

// Prioridad para la tarea maestra
#define ACMETASKPRIO 1
// Tamaño de pila para la tarea maestra
#define ACMETASKSIZE 256

//Globales
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
QueueHandle_t cola_freertos;

EventGroupHandle_t FlagsEventos;

static SemaphoreHandle_t stop_ACME;

static TimerHandle_t CASIO_BMI;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
// Esta funcion se llama si la biblioteca driverlib o FreeRTOS comprueban la existencia de un error (mediante
// las macros ASSERT(...) y configASSERT(...)
// Los parametros nombrefich y linea contienen informacion de en que punto se encuentra el error...
//
//*****************************************************************************
#ifdef DEBUG
void __error__(char *nombrefich, uint32_t linea)
{
    // Si la ejecucion esta aqui dentro, es que el RTOS o alguna de las bibliotecas de perifericos han
    // comprobado que hay un error
    // Mira el arbol de llamadas en el depurador y los valores de nombrefich y linea para encontrar posibles pistas.
    while(1)
    {
    }
}
#endif

//*****************************************************************************
//
// Aqui incluimos los "ganchos" a los diferentes eventos del FreeRTOS
//
//*****************************************************************************

//Esto es lo que se ejecuta cuando el sistema detecta un desbordamiento de pila
//
void vApplicationStackOverflowHook(TaskHandle_t pxTask,  char *pcTaskName)
{
	//
	// This function can not return, so loop forever.  Interrupts are disabled
	// on entry to this function, so no processor interrupts will interrupt
	// this loop.
	while(1)
	{
	}
}

//Esto se ejecuta cada Tick del sistema. LLeva la estadistica de uso de la CPU (tiempo que la CPU ha estado funcionando)
void vApplicationTickHook( void )
{
	static uint8_t count = 0;

	if (++count == 10)
	{
		g_ui32CPUUsage = CPUUsageTick();
		count = 0;
	}
}

//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationIdleHook (void)
{
	SysCtlSleep();
}


//Esto se ejecuta cada vez que entra a funcionar la tarea Idle
void vApplicationMallocFailedHook (void)
{
	while(1);
}

/*  CALLBACK del timer software para el BMI */
void CASIOCallback(TimerHandle_t pxTimer)
{
    MESSAGE_BMI_DATOS_PARAMETER medidas_BMI;
    BMI160_getAcceleration(&medidas_BMI.acc[0], &medidas_BMI.acc[1], &medidas_BMI.acc[2]);
    BMI160_getRotation(&medidas_BMI.gyro[0], &medidas_BMI.gyro[1], &medidas_BMI.gyro[2]);
    remotelink_sendMessage(MESSAGE_BMI_DATOS, (void *)&medidas_BMI, sizeof(medidas_BMI));
}


//*****************************************************************************
//
// A continuacion van las tareas...
//
//*****************************************************************************

static portTASK_FUNCTION(TareaACME,pvParameters)
{
    MESSAGE_ACME_PARAMETER estado_pines;
    MESSAGE_ACME_ADC_PARAMETER estado_adc;
    uint8_t state;

    while(1)
    {
        // Cogemos el semaforo cuando se ha producido una interrupcion del pin PA3
        xSemaphoreTake(stop_ACME, portMAX_DELAY);

        ACME_readInt(&state);

        // El cuarto bit indica si se ha activado el flag del ADC
        if(state & 0x10)
        {
            ACME_readADC(estado_adc.adc_PF);

            remotelink_sendMessage(MESSAGE_ACME_ADC, (void *)&estado_adc, sizeof(estado_adc));
        }

        // Si se activa el bit 1 (GPIO0) o el bit 2 (GPIO1)
        if(state & 0x01 || state & 0x02)
        {
            // Queremos obtener el valor de los pines conectados como entradas
            // Los pones configurados como salida se leeran con el valor logico 0
            ACME_readPin(&estado_pines.GPIO);

            remotelink_sendMessage(MESSAGE_ACME, (void *)&estado_pines, sizeof(estado_pines));
        }

        // Borramos los flags de todos los GPIO
        ACME_clearInt(0xFF);
    }
}


static portTASK_FUNCTION(TareaMaestra,pvParameters)
{
    MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER estado;
    uint32_t ui32Status;

    MuestrasADC muestras;
    MESSAGE_ADC_SAMPLE_PARAMETER parameter_ADC;

    static uint8_t cantidad = 0;
    static uint8_t indice;
    MuestrasADC muestra;
    MESSAGE_64_MUESTRAS_PARAMETER parameter_64;

    EventBits_t eventos;

    while(1)
    {
        eventos = xEventGroupWaitBits(FlagsEventos, BOTONES|ADC0_COLA|ADC1_COLA, pdTRUE, pdFALSE, portMAX_DELAY);

        if(eventos & (BOTONES))
        {
            if (xQueueReceive(cola_freertos, &ui32Status, portMAX_DELAY) == pdTRUE)
            {
                if(ui32Status == 0)
                {
                    UARTprintf("Switch 1 y Switch 2 pulsados\r\n");
                    estado.switch1 = 0;
                    estado.switch2 = 0;
                }
                else if (!(ui32Status & LEFT_BUTTON))
                {
                    UARTprintf("Switch 1 pulsado\r\n");
                    estado.switch1 = 0;
                    estado.switch2 = 1;
                }
                else if (!(ui32Status & RIGHT_BUTTON))
                {
                    UARTprintf("Switch 2 pulsado\r\n");
                    estado.switch2 = 0;
                    estado.switch1 = 1;
                }
                else
                {
                    UARTprintf("Switch 1 o Switch 2 no pulsado\r\n");
                    estado.switch2 = 1;
                    estado.switch1 = 1;
                }

                //Envia el mensaje hacia QT
                remotelink_sendMessage(MESSAGE_ESTADO_SWITCH_EVENTOS,(void *)&estado,sizeof(estado));

            }
        }
        else if(eventos & (ADC0_COLA))
        {
            //Espera y lee muestras del ADC (BLOQUEANTE)
            configADC0_LeeADC0(&muestras);

            //Copia los datos en el parametro (es un poco redundante)
            uint8_t i;
            for(i = 0;i < 6;i++)
            {
                parameter_ADC.chan[i] = muestras.chan[i];
            }

            //Envia el mensaje hacia QT
            remotelink_sendMessage(MESSAGE_ADC_SAMPLE, (void *)&parameter_ADC, sizeof(parameter_ADC));
        }
        else if(eventos & (ADC1_COLA))
        {
            //Espera y lee muestras del ADC (BLOQUEANTE)
            configADC1_LeeADC1(&muestra);

            if(cantidad < 8)
            {
                //Copia los datos en el parametro
                for(indice = 8*cantidad;indice < 8*cantidad + 8;indice++)
                {
                    parameter_64.valor[indice] = muestra.chan[indice-8*cantidad];
                }

                cantidad++;
            }

            if(cantidad == 8)
            {
                cantidad = 0;
                //Envia el mensaje hacia QT
                remotelink_sendMessage(MESSAGE_64_MUESTRAS, (void *)&parameter_64, sizeof(parameter_64));
            }
        }
    }
}

// Funcion callback que procesa los mensajes recibidos desde el PC
// (ejecuta las acciones correspondientes a las ordenes recibidas)
static int32_t messageReceived(uint8_t message_type, void *parameters, int32_t parameterSize)
{
    //Estado de la ejecucion (positivo, sin errores, negativo si error)
    int32_t status = 0;

    //Comprueba el tipo de mensaje
    switch (message_type)
    {
        case MESSAGE_PING:
        {
            status = remotelink_sendMessage(MESSAGE_PING, NULL, 0);
        }

        break;

        case MESSAGE_LED_GPIO:
        {
            MESSAGE_LED_GPIO_PARAMETER parametro;

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro)) > 0)
            {
                GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3,parametro.value);
            }
            else
            {
                //Devuelve un error
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_MODE:
        {
            MESSAGE_MODE_PARAMETER modo;

            if (check_and_extract_command_param(parameters, parameterSize, &modo, sizeof(modo)) > 0)
            {
                if (modo.modorx == 0)
                {
                    UARTprintf(" Modo GPIO \r\n");
                    RGBDisable();
                    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
                }
                else
                {
                    UARTprintf("Modo PWM \r\n");
                    RGBEnable();
                }
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_RGB:
        {
            MESSAGE_RGB_PARAMETER color;

            if (check_and_extract_command_param(parameters, parameterSize, &color, sizeof(color)) > 0)
            {
                uint32_t arrayRGB[3];

                arrayRGB[0] = color.R;
                arrayRGB[1] = color.G;
                arrayRGB[2] = color.B;

                if ((arrayRGB[0] >= 65535) || (arrayRGB[1] >= 65535) || (arrayRGB[2] >= 65535))
                {
                    UARTprintf(" \r\n");
                }
                else{
                    RGBColorSet(arrayRGB);
                }
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_LED_PWM_BRIGHTNESS:
        {
            MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER parametro;

            if (check_and_extract_command_param(parameters, parameterSize, &parametro, sizeof(parametro))>0)
            {
                UARTprintf("Valor: %d\r\n", (int)parametro.rIntensity);
                RGBIntensitySet(parametro.rIntensity);
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_ESTADO_SWITCH:
        {
            MESSAGE_ESTADO_SWITCH_PARAMETER estado;

            if (check_and_extract_command_param(parameters, parameterSize, &estado, sizeof(estado)) > 0)
            {
                UARTprintf("Llegan mensajes para comprobar estados de los switches...\r\n");

                estado.switch1 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4);
                estado.switch2 = GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0);

                remotelink_sendMessage(MESSAGE_ESTADO_SWITCH, (void *)&estado, sizeof(estado));
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_ESTADO_SWITCH_EVENTOS:
        {

            MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER estado;

            if (check_and_extract_command_param(parameters, parameterSize, &estado, sizeof(estado)) > 0)
            {
                if(estado.on_off == 1)
                {
                    GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS, GPIO_BOTH_EDGES);
                    IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);
                    GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
                    IntEnable(INT_GPIOF);
                }
                else
                {
                    // Deshabilitamos las interrupciones de los switches
                    IntDisable(INT_GPIOF);
                    estado.switch1 = 1;
                    estado.switch2 = 1;
                    remotelink_sendMessage(MESSAGE_ESTADO_SWITCH_EVENTOS, (void *)&estado, sizeof(estado));
                }
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_ADC_SAMPLE:
        {
            //Dispara la conversion (por software)
            configADC0_DisparaADC();
        }

        break;

        case MESSAGE_ACTIVAR_MUESTREO:
        {
            MESSAGE_ACTIVAR_MUESTREO_PARAMETER act;

            if (check_and_extract_command_param(parameters, parameterSize, &act, sizeof(act)) > 0)
            {
                configADC1_ConfigTimer(act.activar, act.valor);
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_FRECUENCIA_MUESTREO:
        {
            MESSAGE_FRECUENCIA_MUESTREO_PARAMETER val;

            if (check_and_extract_command_param(parameters, parameterSize, &val, sizeof(val)) > 0)
            {
                UARTprintf("Ha llegado un nuevo valor de frecuencia de muestreo %d\r\n",(int)val.valor);
                configADC1_ConfigTimer(2, val.valor);
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_ACME:
        {
            MESSAGE_ACME_PARAMETER val_acme;

            if (check_and_extract_command_param(parameters, parameterSize, &val_acme, sizeof(val_acme)) > 0)
            {
                UARTprintf("ACME GPIO2 Y 3: %d\r\n",(uint8_t)val_acme.GPIO);
                ACME_writePin(val_acme.GPIO);
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_BMI_ON_OFF:
        {
            MESSAGE_BMI_ON_OFF_PARAMETER val_BMI_on_off;

            if (check_and_extract_command_param(parameters, parameterSize, &val_BMI_on_off, sizeof(val_BMI_on_off)) > 0)
            {
                if(val_BMI_on_off.estado_boton == 1)
                {
                    if(xTimerStart(CASIO_BMI, 0) != pdPASS)
                    {
                        while(1);
                    }
                }
                else
                {
                    if(xTimerStop(CASIO_BMI, 0) != pdPASS)
                    {
                        while(1);
                    }
                }
            }
            else
            {
                status = PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        default:
           //mensaje desconocido/no implementado
           status = PROT_ERROR_UNIMPLEMENTED_COMMAND;
    }

    //Devuelve status
    return status;
}


//*****************************************************************************
//
// Funcion main(), Inicializa los perifericos, crea las tareas, etc... y arranca el bucle del sistema
//
//*****************************************************************************
int main(void)
{
    // Creamos el semaforo para la gestion del ACME
    stop_ACME = xSemaphoreCreateBinary();
    if(stop_ACME == NULL)
    {
        while(1);
    }

    //Crea el grupo de eventos
    FlagsEventos = xEventGroupCreate();
    if( FlagsEventos == NULL )
    {
        while(1);
    }

    cola_freertos = xQueueCreate(16,sizeof(uint32_t));
    if(cola_freertos == NULL)
    {
        while(1);
    }

    // Ponemos el reloj principal a 40 MHz (200 Mhz del Pll dividido por 5)
	MAP_SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

	// Get the system clock speed.
	g_ulSystemClock = SysCtlClockGet();

	// Habilita el clock gating de los perifericos durante el bajo consumo,
	// perifericos que se desee activos en modo Sleep, deben habilitarse con SysCtlPeripheralSleepEnable
	MAP_SysCtlPeripheralClockGating(true);

	// Inicializa el subsistema de medida del uso de CPU (mide el tiempo que la CPU no esta dormida)
	// Para eso utiliza un timer, que aqui hemos puesto que sea el TIMER3 (ultimo parametro que se pasa a la funcion)
	// (y por tanto este no se deberia utilizar para otra cosa).
	CPUUsageInit(g_ulSystemClock, configTICK_RATE_HZ/10, 3);

	// Inicializa los LEDs usando libreria RGB --> usa Timers 0 y 1
	RGBInit(1);
	MAP_SysCtlPeripheralSleepEnable(GREEN_TIMER_PERIPH);
	MAP_SysCtlPeripheralSleepEnable(BLUE_TIMER_PERIPH);
	// Redundante porque BLUE_TIMER_PERIPH y GREEN_TIMER_PERIPH son el mismo
	MAP_SysCtlPeripheralSleepEnable(RED_TIMER_PERIPH);

	//Volvemos a configurar los LEDs en modo GPIO POR Defecto
	MAP_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    //Inicializa los botones (tambien en el puerto F) y habilita sus interrupciones
    ButtonsInit();

    // COMO NO VAMOS A USAR INTERRUPCIONES PARA LA GESTION DE LOS SWITCHES PORQUE LO VAMOS A HACER POR SONDEO
    // NO ES NECESARIO CONFIGURAR LAS INTERRUPCIONES DEL PUERTO F NI TAMPOCO MODIFICAR LA TABLA DE VECTORES
    // DEL FICHERO STARTUP_CCS.C
    // --- CODIGO NO NECESARIO ---
    //GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_FALLING_EDGE);
    //IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);
    //GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
    //IntEnable(INT_GPIOF);

    // Configuracion del puerto PA3 para las interrupciones del ACME
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_3);

    // El PA3 esta asociado a la señal de salida /INT que es activa a nivel bajo, el pin /INT
    // quedará a un nivel bajo (0), volviendo a nivel alto (1) cuando se borren los flags, ver pag.5 datasheet ACME1623
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_FALLING_EDGE);

    IntPrioritySet(INT_GPIOA, configMAX_SYSCALL_INTERRUPT_PRIORITY);
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_PIN_3);
    IntEnable(INT_GPIOA);

	/********************************      Creacion de tareas *********************/

	//Tarea del interprete de comandos (commands.c)
    if (initCommandLine(COMMAND_TASK_STACK,COMMAND_TASK_PRIORITY) != pdTRUE)
    {
        while(1);
    }

	//Esta funcion crea internamente una tarea para las comunicaciones USB.
	//Ademas, inicializa el USB y configura el perfil USB-CDC
	if (remotelink_init(REMOTELINK_TASK_STACK, REMOTELINK_TASK_PRIORITY, messageReceived) != pdTRUE)
	{
	    //Inicializo la aplicacion de comunicacion con el PC (Remote). Ver fichero remotelink.c
	    while(1);
	}

	//Para especificacion 2: Inicialización de los ADC y creación de tareas
	configADC0_IniciaADC0();
	configADC1_IniciaADC1();

    if((xTaskCreate(TareaMaestra, "MasterTask", MASTERTASKSIZE, NULL, tskIDLE_PRIORITY + MASTERTASKPRIO, NULL) != pdTRUE))
    {
        while(1);
    }

    if((xTaskCreate(TareaACME, "ACMETask", ACMETASKSIZE, NULL, tskIDLE_PRIORITY + ACMETASKPRIO, NULL) != pdTRUE))
    {
        while(1);
    }

    CASIO_BMI = xTimerCreate("TIMER_BMI", 40/portTICK_PERIOD_MS, pdTRUE, NULL, CASIOCallback);
    if(CASIO_BMI == NULL){
        while(1);
    }

	// Arranca el  scheduler.  Pasamos a ejecutar las tareas que se hayan activado.
    // el RTOS habilita las interrupciones al entrar aqui, asi que no hace falta habilitarlas
    // De la funcion vTaskStartScheduler no se sale nunca... a partir de aqui pasan a ejecutarse las tareas.
	vTaskStartScheduler();

	while(1)
	{
		// Si llego aqui es que algo raro ha pasado
	}
}

void GPIOFIntHandler(void)
{
    // Hay que inicializarlo a False!!
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    // Lee el estado del puerto (activos a nivel bajo)
    // pasamos el estado de los pines cuando se produjo la interrupcion
    int32_t i32PinStatus = MAP_GPIOPinRead(GPIO_PORTF_BASE, ALL_BUTTONS);

    xEventGroupSetBitsFromISR(FlagsEventos, BOTONES, &higherPriorityTaskWoken);

    // FromISR porque estoy en un rutina de tratamiento de interrupción
    // Pasamos un valor por referencia, escribe en la cola freeRTOS
    xQueueSendFromISR(cola_freertos, &i32PinStatus, &higherPriorityTaskWoken);
    MAP_GPIOIntClear(GPIO_PORTF_BASE, ALL_BUTTONS);

    // Ahora hay que comprobar si hay que hacer el cambio de contexto
    //Se puede hacer con CUALQUIERA de las dos lineas siguientes (las dos hacen lo mismo)
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}

// Manejador del pin PA3 para las interrupciones del ACME
void GPIOInt_A_Handler(void)
{
    BaseType_t higherPriorityTaskWoken = pdFALSE;

    int32_t i32PinStatus = GPIOIntStatus(GPIO_PORTA_BASE, true);

    if(i32PinStatus & GPIO_PIN_3)
    {
        xSemaphoreGiveFromISR(stop_ACME, &higherPriorityTaskWoken);
    }

    MAP_GPIOIntClear(GPIO_PORTA_BASE, GPIO_PIN_3);
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
