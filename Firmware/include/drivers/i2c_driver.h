/**
 * @file i2c_driver.h
 * 
 * @brief Archivo con la definición de funciones para el manejo del driver I2C.
 * 
 * Este archivo contiene la definición de funciones para el manejo del driver I2C, desde
 * la inicialización hasta la lectura y escritura de datos.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see i2c_driver.c
 * @see imu.h
 * @see imu.c
 * 
 * @date 08/11/2024
 * 
 * @version 1.0
 */

#ifndef i2c_driver_H
#define i2c_driver_H

#include "stdio.h"
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"

/*! @brief Puerto I2C--> i2c1*/
#define I2C_PORT (i2c1)

/*! @brief Frecuencia del puerto I2C*/
#define I2C_FREQ 400000

/*! @brief Pin SDA1 */
#define I2C_SDA 6
/*! @brief Pin SCL1 */
#define I2C_SCL 7

/*! @brief Pin de INT1 de la imu*/
#define DOF_INT1 23
/*! @brief Pin de INT2 de la imu*/
#define DOF_INT2 24

// espacio para las instrucciones de preprocesador del rtc


/**
 * @addtogroup I2C_FUNCS
 * @{
 * 
 * Funciones para configurar el driver I2C.
 * 
*/

/**
 * @brief Inicializa el puerto I2C.
 * 
 * Función que inicializa el puerto I2C, los pines SDA y SCL,
 * los pines de interrupción de la IMU y el callback de la interrupción.
 * 
 * si es necesario desde aqui iniciar el rtc
 * 
*/
void smartwatch_i2c_init (void);


/**
 * @brief Escribe un byte en un registro de un dispositivo I2C.
 * 
 * Función que escribe un byte en un registro de un dispositivo I2C.
 * 
 * @param i2c_port Puerto I2C.
 * @param addr Dirección del dispositivo.
 * @param reg Registro del dispositivo.
 * @param value Valor a escribir.
 * 
*/
void i2c_write_byte (i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t value);


/**
 * @brief Escribe varios bytes en un registro de un dispositivo I2C.
 * 
 * Función que escribe varios bytes en un registro de un dispositivo I2C.
 * 
 * @param i2c_port Puerto I2C.
 * @param addr Dirección del dispositivo.
 * @param reg Registro del dispositivo.
 * @param pData puntero de los Datos a escribir.
 * @param len Cantidad de bytes a escribir.
 * 
*/
void i2c_write_nbytes (i2c_inst_t *i2c_port, uint8_t addr, uint8_t *pData, uint32_t len);

/**
 * @brief Lee un byte de un registro de un dispositivo I2C.
 * 
 * Función que lee un byte de un registro de un dispositivo I2C.
 * 
 * @param i2c_port Puerto I2C.
 * @param addr Dirección del dispositivo.
 * @param reg Registro del dispositivo.
 * 
 * @return Valor leído.
 * 
*/
uint8_t i2c_read_byte (i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg);

/**
 * @brief Lee varios bytes de un registro de un dispositivo I2C.
 * 
 * Función que lee varios bytes de un registro de un dispositivo I2C.
 * 
 * @param i2c_port Puerto I2C.
 * @param addr Dirección del dispositivo.
 * @param reg Registro del dispositivo.
 * @param pData puntero de la variable donde se guarda los valores leidos.
 * @param len Cantidad de bytes a leer.
 * 
*/
void i2c_read_nbytes (i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t len);

/**
 * @}
 * 
*/


/**
 * @addtogroup QMI8658A_FUNCS
 * @{
 * 
 * Funciones para configurar el sensor IMU.
 *
*/


/**
 * @brief Configura el sensor IMU.
 * 
 * Función que configura el sensor IMU.
 * 
 * @param ctrl7 Configuración del sensor.
 * 
*/
void imu_pin_setup (void);

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
void gpio_callback (uint gpio, uint32_t events);

/**
 * @}
 * 
*/

#endif