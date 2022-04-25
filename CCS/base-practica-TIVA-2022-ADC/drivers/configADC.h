/*
 * configADC.h
 *
 *  Created on: 22/4/2016
 *      Author: jcgar
 */

#ifndef CONFIGADC_H_
#define CONFIGADC_H_

#include<stdint.h>

typedef struct
{
	uint16_t chan1;
	uint16_t chan2;
	uint16_t chan3;
	uint16_t chan4;
	uint16_t chan5;
	uint16_t chan6;
} MuestrasADC;

typedef struct
{
	uint32_t chan1;
	uint32_t chan2;
	uint32_t chan3;
	uint32_t chan4;
    uint32_t chan5;
    uint32_t chan6;
} MuestrasLeidasADC;

typedef struct
{
    uint16_t chan[8];
} MuestrasADCMicro;

typedef struct
{
    uint32_t chan[8];
} MuestrasLeidaMicro;


void configADC0_ISR(void);
void configADC1_ISR(void);

void configADC0_DisparaADC(void);
void configADC1_ConfigTimer(uint8_t control, double val);

void configADC0_LeeADC0(MuestrasADC *datos);
void configADC1_LeeADC1(MuestrasADCMicro *datos);

void configADC0_IniciaADC0(void);
void configADC1_IniciaADC1(void);

#endif /* CONFIGADC_H_ */
