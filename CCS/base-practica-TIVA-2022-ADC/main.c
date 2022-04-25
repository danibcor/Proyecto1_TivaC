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


//parametros de funcionamiento de la tareas
#define REMOTELINK_TASK_STACK (512)
#define REMOTELINK_TASK_PRIORITY (tskIDLE_PRIORITY+2)
#define COMMAND_TASK_STACK (512)
#define COMMAND_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC0_TASK_STACK (256)
#define ADC0_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define ADC1_TASK_STACK (256)
#define ADC1_TASK_PRIORITY (tskIDLE_PRIORITY+1)
#define SWITCHES_TASK_STACK (256)
#define SWITCHES_TASK_PRIORITY (tskIDLE_PRIORITY+1)

// Prioridad para la tarea SW2TASK
#define SW2TASKPRIO 1
// Tamaño de pila para la tarea SW2TASK
#define SW2TASKSTACKSIZE 128

//Globales
uint32_t g_ui32CPUUsage;
uint32_t g_ulSystemClock;
QueueHandle_t cola_freertos;
uint8_t botonPulsado = 0;

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



//*****************************************************************************
//
// A continuacion van las tareas...
//
//*****************************************************************************
static portTASK_FUNCTION(Switch2Task,pvParameters)
{
    MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER estado;
    uint32_t ui32Status;

    while(1)
    {
        if (xQueueReceive(cola_freertos,&ui32Status,portMAX_DELAY) == pdTRUE){

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
}

//Para especificacion 2. Esta tarea no tendria por que ir en main.c
static portTASK_FUNCTION(ADCTask0, pvParameters)
{

    MuestrasADC muestras;
    MESSAGE_ADC_SAMPLE_PARAMETER parameter;

    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    while(1)
    {
        //Espera y lee muestras del ADC (BLOQUEANTE)
        configADC0_LeeADC0(&muestras);

        //Copia los datos en el parametro (es un poco redundante)
        parameter.chan1 = muestras.chan1;
        parameter.chan2 = muestras.chan2;
        parameter.chan3 = muestras.chan3;
        parameter.chan4 = muestras.chan4;
        parameter.chan5 = muestras.chan5;
        parameter.chan6 = muestras.chan6;

        //Envia el mensaje hacia QT
        remotelink_sendMessage(MESSAGE_ADC_SAMPLE, (void *)&parameter, sizeof(parameter));
    }
}

//Para especificacion 2. Esta tarea no tendria por que ir en main.c
static portTASK_FUNCTION(ADCTask1,pvParameters)
{
    static uint8_t cantidad = 0;
    static uint8_t indice;

    MuestrasADCMicro muestra;
    MESSAGE_64_MUESTRAS_PARAMETER parameter;

    // Bucle infinito, las tareas en FreeRTOS no pueden "acabar", deben "matarse" con la funcion xTaskDelete().
    while(1)
    {
        //Espera y lee muestras del ADC (BLOQUEANTE)
        configADC1_LeeADC1(&muestra);

        if(cantidad < 8)
        {
            //Copia los datos en el parametro
            for(indice = 8*cantidad;indice < 8*cantidad + 8;indice++)
            {
                parameter.valor[indice] = muestra.chan[indice-8*cantidad];
            }

            cantidad++;
        }

        if(cantidad == 8)
        {
            cantidad = 0;
            //Envia el mensaje hacia QT
            remotelink_sendMessage(MESSAGE_64_MUESTRAS, (void *)&parameter, sizeof(parameter));
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
                status=PROT_ERROR_INCORRECT_PARAM_SIZE;
            }
        }

        break;

        case MESSAGE_ESTADO_SWITCH:
        {
            MESSAGE_ESTADO_SWITCH_PARAMETER estado;

            if (check_and_extract_command_param(parameters, parameterSize, &estado, sizeof(estado)) > 0)
            {
                UARTprintf("Llegan mensajes para comprobar estados de los switches...\r\n");

                if((GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0) && (GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) == 0))
                {
                    UARTprintf("Switch 1 y Switch 2 pulsados\r\n");
                    estado.switch1 = 0;
                    estado.switch2 = 0;
                }
                else if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4) == 0)
                {
                    UARTprintf("Switch 1 pulsado\r\n");
                    estado.switch1 = 0;
                    estado.switch2 = 1;
                }
                else if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0) == 0)
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

                remotelink_sendMessage(MESSAGE_ESTADO_SWITCH,(void *)&estado,sizeof(estado));
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
                if(botonPulsado == 0)
                {

                    botonPulsado = 1;

                    GPIOIntTypeSet(GPIO_PORTF_BASE, ALL_BUTTONS,GPIO_FALLING_EDGE);
                    IntPrioritySet(INT_GPIOF,configMAX_SYSCALL_INTERRUPT_PRIORITY);
                    GPIOIntEnable(GPIO_PORTF_BASE,ALL_BUTTONS);
                    IntEnable(INT_GPIOF);

                    if((xTaskCreate(Switch2Task, "Sw2",SW2TASKSTACKSIZE, NULL, tskIDLE_PRIORITY + SW2TASKPRIO, NULL) != pdTRUE))
                    {
                        while(1);
                    }
                }
                else
                {
                    botonPulsado = 0;

                    // Deshabilitamos las interrupciones de los switches
                    IntDisable(INT_GPIOF);
                    estado.switch2 = 1;
                    estado.switch1 = 1;
                    remotelink_sendMessage(MESSAGE_ESTADO_SWITCH_EVENTOS,(void *)&estado,sizeof(estado));
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

    if((xTaskCreate(ADCTask0, (portCHAR *)"ADC0", ADC0_TASK_STACK, NULL, ADC0_TASK_PRIORITY, NULL) != pdTRUE))
    {
        while(1);
    }

    if((xTaskCreate(ADCTask1, (portCHAR *)"ADC1", ADC1_TASK_STACK, NULL, ADC1_TASK_PRIORITY, NULL) != pdTRUE))
    {
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

    // FromISR porque estoy en un rutina de tratamiento de interrupción
    // Pasamos un valor por referencia, escribe en la cola freeRTOS
    xQueueSendFromISR(cola_freertos, &i32PinStatus, &higherPriorityTaskWoken);
    MAP_GPIOIntClear(GPIO_PORTF_BASE, ALL_BUTTONS);

    // Ahora hay que comprobar si hay que hacer el cambio de contexto
    //Se puede hacer con CUALQUIERA de las dos lineas siguientes (las dos hacen lo mismo)
    portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
