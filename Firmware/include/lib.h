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


#include "hardware/gpio.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"

#include "pico/stdlib.h"
#include "pico/platform.h"


#include "./hardware/LCD.h"
#include "./hardware/imu.h"
#include "./drivers/spi_driver.h"
#include "./drivers/i2c_driver.h"


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

typedef struct 
{
    volatile uint8_t step_flag :    1;
    volatile uint8_t wom_flag :     1;

} flags_t;


/**
 * @brief Callback de la interrupción del sensor IMU.
 * 
 * Función que se llama cuando se detecta una interrupción en el sensor IMU.
 * 
 * @param gpio Pin de interrupción.
 * @param events Eventos de interrupción.
 * 
 * @return none
 * 
*/
void gpio_callback(uint gpio, uint32_t events);

/**
 * @brief Configura todo el sistema
 * 
 * Función que configura los clocks del sistema, los pines de interrupción e inicializa las librerias de hardware.
 * 
 * @return none
 * 
*/
void smartwatch_init(void);





#endif