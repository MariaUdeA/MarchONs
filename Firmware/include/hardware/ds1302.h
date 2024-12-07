/**
 * @file ds1302.h
 *
 * @brief Archivo con la definición de funciones y registros para el manejo del módulo DS1302.
 *
 * Este archivo contiene la definición de funciones para el manejo del módulo DS1302, desde
 * la inicialización hasta la lectura de datos.
 * 
 * Mapeo de RAM 31X8:
 * - 0 -> Decimales más significativos del año.
 * 
 * Pinout:
 * - **RST -> CS**: GPIO17
 * - **SCLK -> SCK**: GPIO18
 * - **IO -> MISO**: GPIO16
 * - Conectar MISO con MOSI (GPIO19) mediante una resistencia de 1k.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see ds1302.c
 *
 * @date 21/11/2024
 *
 * @version 0.1
 *
 */


#ifndef DS1302_H
#define DS1302_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/datetime.h"

#include "../../include/drivers/spi_driver.h"

/**< Start RTC bool*/
#define START_RTC 0
/**< Stop RTC bool*/
#define STOP_RTC 1
#define READ_FLAG 1

/**
 * @addtogroup rtc_regs RTC_REGISTERS
 * @{
 *
 * Direcciones de los registros del modulo RTC.
 */
enum RTC_registers{
    /*! \brief Seconds register address*/
    SECONDS_REG=0x80,
    /*! \brief Minutes register address*/
    MINUTES_REG=0x82,
    /*! \brief Hours register address*/
    HOURS_REG=0x84,
    /*! \brief Date register address*/
    DATE_REG=0x86,
    /*! \brief Month register address*/
    MONTH_REG=0x88,
    /*! \brief Day of the week register address*/
    DAY_REG=0x8A,
    /*! \brief Year register address*/
    YEAR_REG=0x8C,
    /*! \brief Write protect register address*/
    WRITE_PROTECT_REG=0x8E,
    /*! \brief Trickle Charger Resistor and Diode Select register address*/
    RES_AND_DIODE_REG=0x90,
    /*! \brief Burst mode address*/
    BURST_MODE_REG=0XBE,
    /*! \brief RAM burst mode address*/
    BURST_MODE_RAM_REG=0XFE,
    /*! \brief RAM start address*/
    RAM_START_ADDR=0XC0,
    /*! \brief RAM end address*/
    RAM_END_ADDR=0XFC,
};

/**
 * @}
 * @addtogroup hour_reg Hours Register
 * @{
 *
 * Campos de bit del registro de horas
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_TCS{
        uint8_t rs_0  : 1;
        uint8_t rs_1  : 1;
        uint8_t ds_0  : 1;
        uint8_t ds_1  : 1;
        uint8_t tcs_0 : 1;
        uint8_t tcs_1 : 1;
        uint8_t tcs_2 : 1;
        uint8_t tcs_3 : 1;
    } BITS; /**< Bitfield Structure */
} _tcs_reg_t;

/**
* @addtogroup RTC_FUNCS
*
* @{
*
* Funciones para configurar el módulo RTC, escribir y leer datos.
*
*/

/**
 * @brief Función para conseguir un registro del RTC.
 * 
 * Esta función lee un registro del modulo rtc.
 * @param regAddress Dirección del registro a leer.
 * 
 * @return Byte leído.
 */
uint8_t getReg(uint8_t regAddress);

/**
 * @brief Función para escribir un registro del RTC.
 * 
 * Esta función escrible un registro del modulo rtc.
 * @param regAddress Dirección del registro a escribir.
 * @param regValue Valor a escribir.
 * 
 * @return none
 */
void setReg(uint8_t regAddress, uint8_t regValue);

/**
 * @brief Función para inicializar el modulo RTC.
 * 
 * Esta función inicializa el modulo rtc.
 * @param backup_time Puntero a estructura de datetime en caso de que el valor del RTC sea incorrecto.
 * 
 */
void DS1302_init(datetime_t* backup_time);

/**
 * @brief Función para determinar si el RTC se puede escribir o no.
 * 
 * @return True, si no se puede escribir, false de lo contrario.
 * 
 */
bool GetIsWriteProtected();

/**
 * @brief Función para permitir o no la escritura del RTC.
 * 
 * @param WriteProtect bool para hacer el write protect, true para bloquear la escritura del RTC, false de lo contrario.
 * @return none
 * 
 */
void SetIsWriteProtected(bool WriteProtect);

/**
 * @brief Función para determinar si el RTC está detenido o no.
 * 
 * @return True, si está detenido, falso de lo contrario.
 * 
 */
bool GetIsRunning();

/**
 * @brief Función para empezar o detener el RTC.
 * 
 * @param run bool, true para empezar el RTC, false para detenerlo.
 * @return none
 * 
 */
void SetIsRunning(bool run);

/**
 * @brief Función para escribir la fecha del modulo RTC.
 * 
 * Esta función escribe la fecha ingresada en el modulo rtc.
 * 
 * @param dt puntero a estructura de tiempo.
 */
void SetDateTime(datetime_t* dt);

/**
 * @brief Función para conseguir la fecha del modulo RTC.
 * 
 * Esta función lee la fecha del modulo rtc.
 * 
 * @param dt puntero a estructura de tiempo a escribir.
 */
void GetDateTime(datetime_t* dt);

/**
 * @brief Función para escribir en la memoria del modulo RTC.
 * 
 * Esta función escribe en la memoria RAM del módulo RTC.
 * 
 * @param memoryAddress posición de la memoria de 0 a 30.
 * @param value valor a escribir en la RAM.
 * 
 */
void SetMemory(uint8_t memoryAddress, uint8_t value);

/**
 * @brief Función para leer una posición de la memoria del modulo RTC.
 * 
 * Esta función lee la memoria RAM del módulo RTC.
 * 
 * @param memoryAddress posición de la memoria de 0 a 30.
 * @return valor leído de la RAM.
 * 
 */
uint8_t GetMemory(uint8_t memoryAddress);

/**
 * @brief Función para verificar la fecha del modulo RTC.
 * 
 * Esta función lee la fecha del modulo RTC y la valida.
 * 
 * @return bool, true si la fecha el válida, false, de lo contrario.
 */
bool IsDateTimeValid();

/**
 * @brief Función para validar una estructura datetime.
 * 
 * @param dt puntero a estructura de tiempo a validar.
 * 
 * @return bool, true si la fecha el válida, false, de lo contrario.
 */
bool DateIsValid(datetime_t* dt);

/**
 * @brief Función para imprimir en serial una estructura datetime.
 * 
 * La estructura TIENE que ser válida, de lo contrario la RP2040 se bloquea.
 * 
 * @param dt puntero a estructura de tiempo a validar.
 * 
 */
void print_datetime(datetime_t* dt);

/**
 * @brief Función para calcular el día de la semana, según la fecha válida.
 * 
 * @param y año, funciona para valores mayores a 1752.
 * @param m month entre 1 y 12.
 * @param d día del mes entre 1 y 31.
 * 
 * @return día de la semana.
 */
uint8_t dayofweek(uint16_t y,uint8_t m,uint8_t d);
/**
 * @}
 *
 */

#endif