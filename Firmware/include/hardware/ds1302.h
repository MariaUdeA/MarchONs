/**
 * @file ds1302.h
 *
 * @brief Archivo con la definición de funciones y registros para el manejo del módulo DS1302.
 *
 * Este archivo contiene la definición de funciones para el manejo del módulo DS1302, desde
 * la inicialización hasta la lectura de datos.
 *
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see ds1302.c
 *
 * @date 21/11/2024
 *
 * @version 1.0
 *
 */


#ifndef DS1302_H
#define DS1302_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/rtc.h"
#include "pico/util/datetime.h"
#include "../../include/drivers/i2c_driver.h"

#define rst_pin 3


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
    /*! \brief Trickle Charger Resistor and Diode Select register address*/
    BURST_MODE_REG=0XBE,
    /*! \brief Trickle Charger Resistor and Diode Select register address*/
    BURST_MODE_RAM_REG=0XFE,
};


/**
 * @}
 * @addtogroup Seg_reg Seconds Register
 * @{
 *
 * Campos de bit del registro de segundos
 */
typedef union{
    uint8_t WORD; /**< Whole Register */
    struct BITS_SEC{
        uint8_t seconds     : 4; /**< Segundos */
        uint8_t ten_seconds : 3; /**< 10 Segundos */
        uint8_t ch          : 1; /**< Clock Halt */
    } BITS; /**< Bitfield Structure */
} _seconds_reg_t; 
#define mSeconds_Clock_Halt 1  /**< Halt Clock !!! */

/**
 * @}
 * @addtogroup min_reg Minutes Register
 * @{
 *
 * Campos de bit del registro de minutos
 */
typedef union{
    uint8_t WORD; /**< Whole Register */
    struct BITS_MIN{
        uint8_t minutes     : 4;   /**< Minutes */
        uint8_t ten_minutes : 3;   /**< 10 Minutes */
        uint8_t             : 1;   /**< Reserved */
    } BITS;  /**< Bitfield Structure */
} _minutes_reg_t;


/**
 * @}
 * @addtogroup hour_reg Hours Register
 * @{
 *
 * Campos de bit del registro de horas
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_HOUR{
        uint8_t hours      : 4; /**< Hours */
        uint8_t ten_hours  : 2; /**< Ten hours or AM/PM */
        uint8_t            : 1;
        uint8_t am_pm      : 1; /**< 12 or 24 selector */
    } BITS; /**< Bitfield Structure */
} _hours_reg_t;


/**
 * @}
 * @addtogroup date_reg Date Register
 * @{
 *
 * Campos de bit del registro de fecha del mes
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_DATE{
        uint8_t date     : 4; /**< Date 1-31 */
        uint8_t ten_date : 2; /**< Ten days */
        uint8_t          : 2; /**< Reserved */
    } BITS; /**< Bitfield Structure */
} _date_reg_t;


/**
 * @}
 * @addtogroup month_reg Month Register
 * @{
 *
 * Campos de bit del registro del mes
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_MONTH{
        uint8_t months     : 4; /**< Months */
        uint8_t ten_months : 1; /**< Ten Months */
        uint8_t            : 3; /**< Reserved */
    } BITS; /**< Bitfield Structure */
} _month_reg_t;


/**
 * @}
 * @addtogroup day_reg Day of the week Register
 * @{
 *
 * Campos de bit del registro del dia de la semana
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_DAY{
        uint8_t day     : 3; /**< Day 1-7 */
        uint8_t         : 5; /**< Reserved */
    } BITS; /**< Bitfield Structure */
} _day_reg_t;


/**
 * @}
 * @addtogroup yearr_reg Year Register
 * @{
 *
 * Campos de bit del registro de los años
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_YR{
        uint8_t years     : 4; /**< years 0-99 */
        uint8_t ten_years : 4; /**< 10 years 0-99 */
    } BITS; /**< Bitfield Structure */
} _year_reg_t;

/**
 * @}
 * @addtogroup wp_reg Write Protect Register
 * @{
 *
 * Campos de bit del registro de write protect
 */
typedef union{
    uint8_t word; /**< Whole Register */
    struct BITS_WP{
        uint8_t      : 7; /**< Reserved */
        uint8_t wp   : 1; /**< Write Protect */
    } BITS; /**< Bitfield Structure */
} _wp_reg_t;
#define clear_wp 0 /**< Write Protect Clear*/

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
 * @brief Función para inicializar el modulo RTC.
 * 
 * Esta función inicializa el modulo rtc.
 *
 */
void DS1302_init(void);
void setup_ds1302(datetime_t * time);
void read_hour();

/**
 * @brief Función para pasar de decimal a BCD.
 * 
 * @param value Valor decimal.
 * @return valor en BCD.
 * 
 */
static inline uint8_t dec_to_bcd(uint8_t value);

/**
 * @brief Función para pasar de BCD a decimal.
 * 
 * @param value Valor en BCD.
 * @return valor en decimal.
 * 
 */
static inline uint8_t bcd_to_dec(uint8_t value);


/**
 * @}
 *
 */

#endif