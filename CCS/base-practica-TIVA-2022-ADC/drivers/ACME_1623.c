/*
 * ACME_1623.c
 *
 *  Created on: 12 feb. 2022
 *      Author: Embedded Systems UMA
 */

#include <stdint.h>
#include <stdbool.h>
#include "drivers/i2c_driver.h"

#define ACME_I2C_ADDRESS 0x2C

#define ACME_ID_REGISTER 0
#define ACME_DIR_REGISTER 1
#define ACME_OUTPUT_REGISTER 2
#define ACME_INPUT_REGISTER 3
#define ACME_INT_TYPE_REGISTER 4
#define ACME_ADC_INT_TRIGGER_REGISTER 5
#define ACME_INT_CLEAR_REGISTER 6
#define ACME_INT_STATUS_REGISTER 7

#define ACME_CH0_LOW 8
#define ACME_CH0_HIGH 9
#define ACME_CH1_LOW 10
#define ACME_CH1_HIGH 11
#define ACME_CH2_LOW 12
#define ACME_CH2_HIGH 13
#define ACME_CH3_LOW 14
#define ACME_CH3_HIGH 15

#define ACME_ID_REGISTER_VALUE 0xCD

//Esta funcion debe comprobar si el sensor esta o no disponible y habilitar las salidas que indiquemos
int32_t ACME_initDevice(void)
{
    uint8_t array_envio[2];
    uint8_t rxvalue;
    int32_t status;

    array_envio[0]=ACME_ID_REGISTER;
    status=I2CDriver_WriteAndRead(ACME_I2C_ADDRESS,array_envio,1,&rxvalue,1);
    if ((status!=0)||(rxvalue!=ACME_ID_REGISTER_VALUE))
            return -1; // XXX Esta operación va a fallar hasta que se arregle la biblioteca i2c_driver.c

    array_envio[0]=ACME_DIR_REGISTER;
    array_envio[1]=0;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    if (status!=0)
       return -1;

    array_envio[0]=ACME_INT_TYPE_REGISTER;
    array_envio[1]=0;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;

    array_envio[0]=ACME_ADC_INT_TRIGGER_REGISTER;
    array_envio[1]=0;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;

    array_envio[0]=ACME_INT_CLEAR_REGISTER;
    array_envio[1]=0x3F;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    if (status!=0)
            return -1;


    return status;
}

int32_t ACME_setPinDir (uint8_t dir)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_DIR_REGISTER;
    array_envio[1]=dir;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    return status;
}


int32_t ACME_writePin (uint8_t pin)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_OUTPUT_REGISTER;
    array_envio[1]=pin;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    return status;
}

int32_t ACME_readPin (uint8_t *pin)
{
    uint8_t dir_register=ACME_INPUT_REGISTER;
    int32_t status;

    status=I2CDriver_WriteAndRead(ACME_I2C_ADDRESS,&dir_register,sizeof(dir_register),pin,sizeof(uint8_t));
    return status;
}

int32_t ACME_clearInt (uint8_t pin)
{
    uint8_t array_envio[2];
    int32_t status;

    array_envio[0]=ACME_INT_CLEAR_REGISTER;
    array_envio[1]=pin;
    status=I2CDriver_Write(ACME_I2C_ADDRESS,array_envio,sizeof(array_envio));
    return status;
}


//Escribe los registros INT_TYPE y ADC_INT_TRIG
// A COMPLETAR
ACME_setIntTriggerType (uint8_t intType, uint8_t intTrigADC)
{
    uint8_t array_escritura[3];
    int32_t status;

    array_escritura[0] = ACME_INT_TYPE_REGISTER;
    array_escritura[1] = intType;
    array_escritura[2] = intTrigADC;

    status = I2CDriver_Write(ACME_I2C_ADDRESS, array_escritura, sizeof(array_escritura));

    if(status!=0)
    {
        return -1;
    }

    return status;
}

int32_t ACME_readInt (uint8_t *pin)
{
    uint8_t dir_register = ACME_INT_STATUS_REGISTER;
    int32_t status;

    status = I2CDriver_WriteAndRead(ACME_I2C_ADDRESS, &dir_register, sizeof(dir_register), pin, sizeof(uint8_t));
    return status;
}

// Lee los datos de los ADC y los guarda en el array adcData (el array que se le pase debe tener 4 posiciones de tipo uint16_t
// A COMPLETAR
int32_t ACME_readADC (uint16_t *adcData)
{
    uint8_t dir_registro_adc = ACME_CH0_LOW;
    int32_t status;
    uint8_t dato_rec[8];

    status = I2CDriver_WriteAndRead(ACME_I2C_ADDRESS, &dir_registro_adc, sizeof(dir_registro_adc), dato_rec, sizeof(uint8_t)*8);

    adcData[0] = (((uint16_t)dato_rec[1]) << 8) | dato_rec[0];
    adcData[1] = (((uint16_t)dato_rec[3]) << 8) | dato_rec[2];
    adcData[2] = (((uint16_t)dato_rec[5]) << 8) | dato_rec[4];
    adcData[3] = (((uint16_t)dato_rec[7]) << 8) | dato_rec[6];

    if(status!=0)
    {
        return -1;
    }

    return status;
}



//Funcion para probar la lectura...
int32_t ACME_read (uint8_t *bytes,uint8_t size)
{
    int32_t status;
    status = I2CDriver_Read(ACME_I2C_ADDRESS,bytes,size);
    return status;
}
