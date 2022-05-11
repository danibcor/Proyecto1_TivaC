#ifndef RL_MESSAGES_H
#define RL_MESSAGES_H

//Codigos de los mensajes y definicion de parametros para el protocolo RPC
// El estudiante debe agregar aqui cada nuevo mensaje que implemente. IMPORTANTE el orden de los mensajes
// debe SER EL MISMO aqui, y en el codigo equivalente en la parte del microcontrolador (Code Composer)

#include <cstdint>
typedef enum {
    MESSAGE_REJECTED,
    MESSAGE_PING,
    MESSAGE_LED_GPIO,
    MESSAGE_LED_PWM_BRIGHTNESS,
    MESSAGE_ADC_SAMPLE,
    MESSAGE_MODE,
    MESSAGE_RGB,
    MESSAGE_ESTADO_SWITCH,
    MESSAGE_ESTADO_SWITCH_EVENTOS,
    MESSAGE_ACTIVAR_MUESTREO,
    MESSAGE_FRECUENCIA_MUESTREO,
    MESSAGE_64_MUESTRAS,
    MESSAGE_ACME,
    MESSAGE_ACME_ADC,
    MESSAGE_BMI_ON_OFF,
    MESSAGE_BMI_DATOS,
} messageTypes;

//Estructuras relacionadas con los parametros de los mensahes. El estuadiante debera crear las
// estructuras adecuadas a los comandos usados, y asegurarse de su compatibilidad en ambos extremos

#pragma pack(1) //Cambia el alineamiento de datos en memoria a 1 byte.

typedef struct{
    int16_t acc[3];
    int16_t gyro[3];
} MESSAGE_BMI_DATOS_PARAMETER;

typedef struct{
    uint8_t estado_boton;
} MESSAGE_BMI_ON_OFF_PARAMETER;

typedef struct{
    uint16_t adc_PF[4];
} MESSAGE_ACME_ADC_PARAMETER;

typedef struct{
    uint8_t GPIO;
} MESSAGE_ACME_PARAMETER;

typedef struct{
    uint16_t valor[64];
} MESSAGE_64_MUESTRAS_PARAMETER;

typedef struct{
    uint8_t activar;
    double valor;
} MESSAGE_ACTIVAR_MUESTREO_PARAMETER;

typedef struct{
    double valor;
} MESSAGE_FRECUENCIA_MUESTREO_PARAMETER;

typedef struct{
    uint8_t on_off;
    uint8_t switch1;
    uint8_t switch2;
} MESSAGE_ESTADO_SWITCH_EVENTOS_PARAMETER;

typedef struct{
    uint8_t switch1;
    uint8_t switch2;
} MESSAGE_ESTADO_SWITCH_PARAMETER;

typedef struct{
    uint8_t R;
    uint8_t G;
    uint8_t B;
} MESSAGE_RGB_PARAMETER;

typedef struct{
    uint8_t modotx;
} MESSAGE_MODE_PARAMETER;

typedef struct {
    uint8_t command;
} MESSAGE_REJECTED_PARAMETER;

typedef union{
    struct {
         uint8_t padding:1;
         uint8_t red:1;
         uint8_t blue:1;
         uint8_t green:1;
    } leds;
    uint8_t value;
} MESSAGE_LED_GPIO_PARAMETER;

typedef struct {
    float rIntensity;
} MESSAGE_LED_PWM_BRIGHTNESS_PARAMETER;

typedef struct
{
    uint16_t chan[6];
} MESSAGE_ADC_SAMPLE_PARAMETER;

#pragma pack()  //...Pero solo para los comandos que voy a intercambiar, no para el resto.

#endif // RPCCOMMANDS_H


