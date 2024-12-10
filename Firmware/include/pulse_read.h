/**
 * @file pulse_read.h
 * 
 * @brief Archivo con la definición de funciones para el manejo del medidor de pulso.
 * 
 * Este archivo contiene la definición de funciones para el manejo del medidor de pulso, la configuración
 * del pin de interrupcion y la medición del pulso.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see pulse_read.c
 * @see max30102.h
 * @see max30102.c
 * 
 * @date 04/12/2024
 * 
 * @version 1.0
 */

#ifndef PULSE_READ_H
    #define PULSE_READ_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "./hardware/max30102.h"

/**< Maximum data window*/
#define MAX_WINDOW 255
/**
 * 
 * @addtogroup beat_struct Beat Detector Structure
 * @{
 *
 * Estructura del detector de pulso
 */
typedef struct beat_detector
{
    uint16_t sample_rate; //100
    uint8_t window_size; //10
    uint8_t smoothing_window; //5
    uint32_t sample[MAX_WINDOW];
    uint32_t timestamps[MAX_WINDOW];
    uint32_t filtered_samples[MAX_WINDOW+1];
    uint8_t round;
    uint8_t peak_len;
} beat_detector_t;
/**
 * @}
 */

/**
 * @brief Función para guardar un sample de IR.
 * 
 * Esta función guarda una sample de IR y la suaviza.
 * @param sample sample de IR  a guardar.
 * 
 * @return None.
 */
void add_sample(uint32_t sample);

/**
 * @brief Función medir el pulso por minuto.
 * 
 * Esta función toma el banco, encuentra los picos y su distancia para encontrar
 * el pulso por minuto.
 * 
 * @return beats per minute.
 */
uint8_t calculate_heart_rate();


#endif
