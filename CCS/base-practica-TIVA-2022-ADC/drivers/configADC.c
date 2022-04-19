#include<stdint.h>
#include<stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/timer.h"
#include "configADC.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Al ser una cola static, la cola solo se puede emplear en este fichero
static QueueHandle_t cola_adc;

//Provoca el disparo de una conversion (hemos configurado el ADC con "disparo software" (Processor trigger)
void configADC_DisparaADC(void)
{
	ADCProcessorTrigger(ADC0_BASE,1);
}


void configADC_IniciaADC(void)
{
			    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
			    // Especificamos que el periferico siga encendido en modo de bajo consumo
			    SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_ADC0);

				//HABILITAMOS EL GPIOE, GPIOB y GPIOD
			    // Para configurar el multiplexor de señales
				SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE | SYSCTL_PERIPH_GPIOB | SYSCTL_PERIPH_GPIOD);
				SysCtlPeripheralSleepEnable(SYSCTL_PERIPH_GPIOE | SYSCTL_PERIPH_GPIOB | SYSCTL_PERIPH_GPIOD);

				// Enable pin PE3 for ADC AIN0|AIN1|AIN2|AIN3
				GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_0);
				// Enable pin PD3 for ADC AIN4
				GPIOPinTypeADC(GPIO_PORTD_BASE, GPIO_PIN_3);
				// Enable pin PB5 for ADC AIN11
				GPIOPinTypeADC(GPIO_PORTB_BASE, GPIO_PIN_5);

				//CONFIGURAR SECUENCIADOR 1
				//ADCSequenceDisable(ADC0_BASE,1);

				// Vamos a configurar el secuenciador 0 porque admite 8 pasos, nosotros tenemos 6
				// Secuenciador 0 -> admite 8, Secuenciador 1/2 -> admite 4 y Secuenciador 3 -> admite 1
				ADCSequenceDisable(ADC0_BASE, 0);

				//Configuramos la velocidad de conversion al maximo (1MS/s)
				// 1us por cada muestra
				ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_RATE_FULL, 1);

				ADCSequenceConfigure(ADC0_BASE,0,ADC_TRIGGER_PROCESSOR,0);	//Disparo software (processor trigger)
				ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH0);
				ADCSequenceStepConfigure(ADC0_BASE, 0, 1, ADC_CTL_CH1);
				ADCSequenceStepConfigure(ADC0_BASE, 0, 2, ADC_CTL_CH2);
				ADCSequenceStepConfigure(ADC0_BASE, 0, 3, ADC_CTL_CH3);
				ADCSequenceStepConfigure(ADC0_BASE, 0, 4, ADC_CTL_CH4);
				ADCSequenceStepConfigure(ADC0_BASE, 0, 5, ADC_CTL_CH11 | ADC_CTL_IE | ADC_CTL_END);	//La ultima muestra provoca la interrupcion
				ADCSequenceEnable(ADC0_BASE,1); //ACTIVO LA SECUENCIA

				//Habilita las interrupciones
				ADCIntClear(ADC0_BASE, 1);
				ADCIntEnable(ADC0_BASE, 1);
				IntPrioritySet(INT_ADC0SS1, configMAX_SYSCALL_INTERRUPT_PRIORITY);
				IntEnable(INT_ADC0SS1);

				//Creamos una cola de mensajes para la comunicacion entre la ISR y la tara que llame a configADC_LeeADC(...)
				cola_adc=xQueueCreate(8,sizeof(MuestrasADC));
				if (cola_adc==NULL)
				{
					while(1);
				}
}


void configADC_LeeADC(MuestrasADC *datos)
{
	xQueueReceive(cola_adc,datos,portMAX_DELAY);
}

void configADC_ISR(void)
{
	portBASE_TYPE higherPriorityTaskWoken=pdFALSE;

	MuestrasLeidasADC leidas;
	MuestrasADC finales;
	ADCIntClear(ADC0_BASE,1);//LIMPIAMOS EL FLAG DE INTERRUPCIONES

	// Hay que tener cuidado con el tipo empleado para coger los datos guardados
	ADCSequenceDataGet(ADC0_BASE,1,(uint32_t *)&leidas);//COGEMOS LOS DATOS GUARDADOS

	//Pasamos de 32 bits a 16 (el conversor es de 12 bits, así que sólo son significativos los bits del 0 al 11)
	// valores de entre 0 hasta 4096 ()
	finales.chan1=leidas.chan1;
	finales.chan2=leidas.chan2;
	finales.chan3=leidas.chan3;
	finales.chan4=leidas.chan4;

	//Guardamos en la cola
	xQueueSendFromISR(cola_adc,&finales,&higherPriorityTaskWoken);
	portEND_SWITCHING_ISR(higherPriorityTaskWoken);
}
