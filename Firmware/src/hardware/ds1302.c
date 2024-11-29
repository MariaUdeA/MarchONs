/**
 * @file ds1302.c
 *
 * @brief Archivo con la implementación de funciones para el manejo del modulo RTC.
 *
 * Este archivo contiene la implementación de funciones para el manejo del módulo RTC, desde
 * la inicialización hasta la lectura de datos.
 *
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see ds1302.h
 *
 * @date 21/11/2024
 *
 * @version 1.0
 *
 *
 **/

#include "../../include/hardware/ds1302.h"


//Como se trabajan los números en BCD en el RTC se generan estas funciones
static inline uint8_t dec_to_bcd(uint8_t value){
    return (value / 10)<<4+(value % 10);
}

static inline uint8_t bcd_to_dec(uint8_t value){
    return (value>>4 * 10)+(value | 0xF);
}
//En teoria
//solo es 1<<7 & REG & R/W PARA EL COMMAND BYTE


