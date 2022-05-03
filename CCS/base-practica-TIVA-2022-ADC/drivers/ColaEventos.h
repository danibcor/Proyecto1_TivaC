/*
 * ColaEventos.h
 *
 *  Created on: 26 abr. 2022
 *      Author: Daniel & Juanjo
 */

#ifndef COLAEVENTOS_H_
#define COLAEVENTOS_H_


#include "event_groups.h"

// Manejador para el flag de eventos
extern EventGroupHandle_t FlagsEventos;

// Etiquetas de eventos
#define BOTONES 0x0001
#define ADC0_COLA 0x0002
#define ADC1_COLA 0x0004


#endif /* COLAEVENTOS_H_ */
