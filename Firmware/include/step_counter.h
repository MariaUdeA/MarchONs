/**
 * @file step_counter.h
 * 
 * @brief Archivo con la definición de funciones para el manejo del contador de pasos.
 * 
 * Este archivo contiene la definición de funciones para el manejo del contador de pasos, la configuración
 * del pin de interrupcion y la lectura de pasos.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see step_counter.c
 * @see imu.h
 * @see imu.c
 * 
 * @date 08/11/2024
 * 
 * @version 1.0
 */

#ifndef STEP_COUNTER_H
    #define STEP_COUNTER_H

    #include "pico/stdlib.h"
    #include "hardware/gpio.h"

    #define STEP_PIN 23          /**< Pin de interrupción del contador de pasos. */

    /**
     * @brief Callback de la interrupción del contador de pasos.
     * 
     * Esta función es llamada cuando se genera una interrupción en el contador de pasos.
     * 
     * @return 
     */
    void step_counter_callback (uint gpio, uint32_t events);
    
    
    /**
     * @brief Función que configura el pin de interrupción del contador de pasos.
     * 
     * Esta función configura el pin de interrupción del contador de pasos.
     * 
     * @return void
     */
    void configure_step_pin (void);
    

    /**
     * @brief Funcion que lee el número de pasos.
     * 
     * Esta función lee el número de pasos del contador de pasos.
     * 
     * @return uint32_t Número de pasos.
     */
    uint32_t read_steps (void);







#endif