/**
 * @file lib.h
 * 
 * @brief Archivo que contiene las cabeceras de las funciones del smartwatch.
 * 
 * Este archivo contiene la libreria de las funciones del smartwatch, es una libreria de
 * alto nivel que contiene las funciones de inicialización y configuración del smartwatch. utiliza el SDK
 * de pico para la configuración de los pines y logica del smartwatch.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see Firmware.c
 * 
 * @date 25/11/2024
 * 
 */

#ifndef LIB_H
#define LIB_H

#include <stdio.h>

// Librerias del SDK
#include "hardware/gpio.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"


// Librerias de la pico
#include "pico/stdlib.h"
#include "pico/platform.h"

// Librerias de la aplicacion
#include "./hardware/LCD.h"
#include "./hardware/imu.h"
#include "./hardware/ds1302.h"
#include "./hardware/max30102.h"
#include "./drivers/spi_driver.h"
#include "./drivers/i2c_driver.h"
#include "./pulse_read.h"


//Libreria LGVL para el manejo de la interfaz grafica
#include "../lv_conf.h"
#include "../lvgl/lvgl.h"


/*! @brief Frecuencia del PLL del sistema */
#define PLL_SYS_KHZ (270 * 1000)
/*! @brief Pin de interrupción 1 */
#define INT1_PIN 23
/*! @brief Mascara para la interupcion por WoM*/
#define WOM_INT 0x04
/*! @brief Mascara para la interrupcion por paso */
#define STEP_INT 0x10
/*! @brief Resolución horizontal del display */
#define DISP_HOR_RES 240 
/*! @brief Resolución vertical del display */
#define DISP_VER_RES 240 


typedef struct 
{
    volatile uint8_t step_flag :    1;
    volatile uint8_t wom_flag :     1;

} flags_t;

/**
 * @brief Estructura para el manejo de las interrupciones.
 * 
 */
typedef struct{
    uint8_t five_mil :1;
    uint8_t half     :1;
    uint8_t full     :1;
    uint8_t one_half :1;
    uint8_t          :4;
}timer_flags_t;

/**
 * @brief Función que actualiza la pantalla principal
 * 
 * En esta funcion se realiza el chequeo de las variables y banderas de software para actualizar la pantalla principal
 * 
 * @return none
 * 
 */
void update_main_screen(void);


/**
 * @brief Configura todo el sistema
 * 
 * Función que configura los clocks del sistema, los pines de interrupción e inicializa las librerias de hardware.
 * 
 * @return none
 * 
*/
void smartwatch_init(void);

/**
 * @brief Mira si es un nuevo día
 * 
 * Función que revisa si hay cambio de día. Si lo hay, reinicia la imu y la memoria del RTC.
 * 
 * @param now puntero a la estructura que guarda el tiempo actual.
 * 
*/
void check_for_new_day(datetime_t*now);

/**
 * @brief Guarda los pasos en la memoria del RTC y actualiza el valor para ser mostrado en pantalla.
 * 
 * 
 * @param offset Valor guardado de pasos en el RTC cuando se enciende el smartwatch.
 * @param steps puntero al valor de los pasos de la IMU.
 * 
*/
void save_steps(uint32_t offset, uint32_t*steps);

/**
 * @brief Función main del programa.
 * 
 * Función que inicializa y se queda en el loop del programa.
 * 
 * 
*/
int smartwatch_main(void);







#endif