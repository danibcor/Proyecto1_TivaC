/*
 * ACME_1623.h
 *
 *  Created on: 12 feb. 2022
 *      Author: Embedded Systems UMA
 */

#ifndef ACME_1623_H_
#define ACME_1623_H_

extern int32_t ACME_initDevice(void);
extern int32_t ACME_setPinDir (uint8_t dir);
extern int32_t ACME_writePin (uint8_t pin);
extern int32_t ACME_readPin (uint8_t *pin);
extern int32_t ACME_clearInt (uint8_t pin);
extern int32_t ACME_setIntTriggerType (uint8_t intGPIO,uint8_t intTrigADC);
extern int32_t ACME_readInt (uint8_t *pin);
extern int32_t ACME_read (uint8_t *bytes,uint8_t size);
extern int32_t ACME_readADC (uint16_t *adcData);
#endif /* ACME_3416_H_ */
