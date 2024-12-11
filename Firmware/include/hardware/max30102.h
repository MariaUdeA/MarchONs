/**
 * @file max30102.h
 *
 * @brief Archivo con la definición de funciones y registros para el manejo del módulo MAX30102.
 *
 * Este archivo contiene la definición de funciones para el manejo del módulo DS1302, desde
 * la inicialización hasta la lectura de datos.
 * 
 * Posee una FIFO de 32x8
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see max3012.c
 *
 * @date 3/12/2024
 *
 * @version 0.1
 *
 */

#ifndef MAX30102_H
#define MAX30102_H

#include <stdio.h>
#include "pico/stdlib.h"

#include "../../include/drivers/i2c_driver.h"

/**< MAX30102 Interrupt Pin, Active Low!!*/
/**< MAX30102 Address*/
#define MAX_ADDR 0x57
/**< MAX multi mode Bit Field*/
#define MAX_MODE_MULTI 0x03
/**< MAX part ID WHO AM I*/
#define MAX_PART_ID 0x15
/**< Maximum buffer length*/
#define I2C_BUFFER_LENGTH 256

/**
 * @addtogroup rtc_regs RTC_REGISTERS
 * @{
 *
 * Direcciones de los registros del modulo RTC.
 */
enum MAX_registers{
    /*! \brief R2C, IRQ status 1*/
    IRQ_STATUS1_REG=0x0,
    /*! \brief R2C, IRQ status 2, this just has the temp interrupt status*/
    IRQ_STATUS2_REG,
    /*! \brief RW, IRQ enable 1*/
    IRQ_EN1_REG,
    /*! \brief RW, IRQ enable 2, this just has the temp interrupt enable*/
    IRQ_EN2_REG,
    /*! \brief RW, Pointer where the MAX30102 writes the next sample*/
    FIFO_WR_PTR_REG,
    /*! \brief RW, Number of samples lost if the FIFO is full*/
    OVERFLOW_COUNTER_REG,
    /*! \brief RW, pointer where the MCU gets the next sample from the FIFO*/
    FIFO_RD_PTR_REG,
    /*! \brief RW, next sample to be read */
    FIFO_DATA_REG,
    /*! \brief RW, Register for the FIFO configuration*/
    FIFO_CONFIG_REG,
    /*! \brief RW, Power save mode, reset, and function modes*/
    MODE_CONFIG_REG,
    /*! \brief R/W, SPO2 configuration */
    SPO2_CONFIG_REG,
    /*! \brief R/W, LED1 Pulse amplitude 0mA-51mA*/
    LED1_PULSE_AMP_REG=0XC,
    /*! \brief R/W, LED2 Pulse amplitude 0mA-51mA*/
    LED2_PULSE_AMP_REG,
    /*! \brief R/W, SLOT1 and SLOT2 LED active*/
    MLED_MODE_CTRL1_REG=0X11,
    /*! \brief R/W, SLOT3 and SLOT4 LED active*/
    MLED_MODE_CTRL2_REG,
    /*! \brief RO, Integer temperature register, includes negatives*/
    DIE_TEMP_INT_REG=0X1F,
    /*! \brief RO, fractional temperature register, it goes in increments of 0.0625C*/
    DIE_TEMP_FRAC_REG,
    /*! \brief RW, temperature enable register, self clearing*/
    DIE_TEMP_CONFIG_REG,
    /*! \brief RO, revision ID register*/
    REVISION_ID_REG=0XFE,
    /*! \brief RO, part ID register, POR value is 0x15*/
    PART_ID_REG,

};

/**
 * @}
 * 
 * @addtogroup SamplingRate Sampling Rates
 * @{
 *
 * Enumeración de los Sampling rates con su campo de bits respectivo.
 * 
 */
typedef enum SamplingRate {
    MAX_SR_50HZ      = 0x00,
	MAX_SR_100HZ     = 0x01,
	MAX_SR_200HZ     = 0x02,
	MAX_SR_400HZ     = 0x03,
	MAX_SR_800HZ     = 0x04,
	MAX_SR_1000HZ    = 0x05,
	MAX_SR_1600HZ    = 0x06,
	MAX_SR_3200HZ    = 0x07
} SamplingRate;

/**
 * @}
 * 
 * @addtogroup LEDPulseWidth LED Pulse Widths
 * @{
 *
 * Enumeración de los Pulse Widths con su campo de bits respectivo.
 * 
 */
typedef enum LEDPulseWidth {
    MAX_PW_70US_15BITS    = 0x00,
    MAX_PW_120US_16BITS    = 0x01,
    MAX_PW_215US_17BITS    = 0x02,
    MAX_PW_410US_18BITS   = 0x03
} LEDPulseWidth;

/**
 * @}
 * 
 * @addtogroup SampleAverage Sample Averages
 * @{
 *
 * Enumeración de los Sample Averages de la FIFO con su campo de bits respectivo.
 * 
 */
typedef enum SampleAverage {
    MAX_SA_1SAMPLE    = 0x00,
    MAX_SA_2SAMPLE,
    MAX_SA_4SAMPLE,
    MAX_SA_8SAMPLE,
    MAX_SA_16SAMPLE,
    MAX_SA_32SAMPLE,
} SampleAverage;

/**
 * @}
 * 
 * @addtogroup ADC_RGE ADC Ranges
 * @{
 *
 * Enumeración de los rangos del ADC con su campo de bits respectivo.
 * 
 */
typedef enum ADC_RGE {
    MAX_ADC_RGE_2048    = 0x00,
    MAX_ADC_RGE_4096,
    MAX_ADC_RGE_8192,
    MAX_ADC_RGE_16384
} ADC_RGE;

/**
 * @}
 * 
 * @addtogroup LEDCurrent LED Currents
 * @{
 *
 * Enumeración de algunas de las corrientes del LED con su campo de bits respectivo.
 * 
 */
typedef enum LEDCurrent {
	MAX30102_LED_CURR_0MA      = 0x00,
	MAX30102_LED_CURR_4_4MA    = 0x10,
	MAX30102_LED_CURR_7_6MA    = 0x20,
	MAX30102_LED_CURR_11MA     = 0x30,
	MAX30102_LED_CURR_14_2MA   = 0x40,
	MAX30102_LED_CURR_17_4MA   = 0x50,
	MAX30102_LED_CURR_20_8MA   = 0x60,
	MAX30102_LED_CURR_24MA     = 0x70,
	MAX30102_LED_CURR_27_1MA   = 0x80,
	MAX30102_LED_CURR_30_6MA   = 0x90,
	MAX30102_LED_CURR_33_8MA   = 0xa0,
	MAX30102_LED_CURR_37MA     = 0xb0,
	MAX30102_LED_CURR_40_2MA   = 0xc0,
	MAX30102_LED_CURR_43_6MA   = 0xd0,
	MAX30102_LED_CURR_46_8MA   = 0xe0,
	MAX30102_LED_CURR_50MA     = 0xf0
} LEDCurrent;

/**
 * @}
 * 
 * @addtogroup SLOTS Slots
 * @{
 *
 * Enumeración de los LEDs de los SLOTS con su campo de bits respectivo.
 * 
 */
typedef enum SLOTS{
    SLOT_NONE=0,
    SLOT_RED_LED,
    SLOT_IR_LED,
}LED_SLOTS;

/**
 * @}
 * 
 * @addtogroup SPO2_CONFIG Configuración SPO2
 * @{
 *
 * Estructura del registro de configuración de SPO2.
 * 
 */
typedef union SPO2_CONFIG{   
    uint8_t WORD;
    struct BITS_SPO2{
        uint8_t led_pw      :2;
        uint8_t sample_rate :3;
        uint8_t adc_range   :1;
        uint8_t             :1;
    } BITS;
} SPO2_config_t;

/**
 * @}
 * 
 * @addtogroup FIFO_CONFIG Configuración FIFO
 * @{
 *
 * Estructura del registro de configuración de la FIFO.
 * 
 */
typedef union FIFO_CONFIG{   
    uint8_t WORD;
    struct BITS_FIFO{
        uint8_t FIFO_AFULL    :4;
        uint8_t FIFO_ROLLOVER :1;
        uint8_t SAMPLE_AVG    :3;
    } BITS;
} FIFO_config_t;

/**
 * @}
 * 
 * @addtogroup record Buffer circular
 * @{
 *
 * Estructura del buffer circular de 4 posiciones.
 * 
 */
typedef struct Record
{
    uint32_t red[4];
    uint32_t IR[4];
    uint8_t head;
    uint8_t tail;
} sense_struct_t; //This is our circular buffer of readings from the sensor
/**
 * @}
 */

bool pulse_getIR_flag();

void pulse_setIR_flag(bool set);

/**
 * @brief Función para inicializar el módulo MAX30102.
 * 
 * Esta función configura los modos de los LEDs, el SPO2, los SLOTS, la corriente de los leds y la FIFO.
 * 
 * @return none.
 */
void max_init(bool* read_ir);

/**
 * @brief Función que regresa la última medida leída de la FIFO.
 *
 * 
 * @return medida del IR de 32 bits.
 */
uint32_t pulse_getIR(void);

#endif