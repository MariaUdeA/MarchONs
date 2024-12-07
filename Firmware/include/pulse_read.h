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

#define AVG_SIZE 4


void get_bpm(void);
void add_sample(uint32_t sample);
uint8_t calculate_heart_rate();


#endif
