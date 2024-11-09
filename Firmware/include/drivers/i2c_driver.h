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

    #include "pico/stdlib.h"
    #include "hardware/i2c.h"

    #define IMU_ADDR 0x6B       /**< Dirección del sensor IMU (Ver el datashee del QMI8658A pg. 9)*/


    /**
     * @addtogroup I2C_CONFIG
     * 
     * @{
     * 
     * Configuración del driver I2C.
     */
    #define I2C_PORT i2c1       /**< Puerto I2C a usar. */
    #define I2C_SDA 6           /**< Pin SDA del puerto I2C. */
    #define I2C_SCL 7           /**< Pin SCL del puerto I2C. */
    #define I2C_FREQ 400000     /**< Frecuencia del puerto I2C. */
    /**
     * @}
     * 
    */

    /**
     * @brief Función que inicializa el driver I2C.
     * 
     * Esta función inicializa el driver I2C, configurando los registros necesarios para su correcto funcionamiento.
     * 
     * @return void
     */
    void i2c_init_setup(void);

    /**
     * @brief Función que escribe un registro en el sensor IMU.
     * 
     * Esta función escribe un registro del sensor IMU.
     * 
     * @param reg Registro a leer.
     * @param data Dato a escribir en el registro.
     * 
     */
    void write_register_imu(uint8_t reg, uint8_t data);

    /**
     * @brief Función que lee un registro del sensor IMU.
     * 
     * Esta función lee un registro del sensor IMU.
     * 
     * @param reg Registro a leer.
     * 
     * @return uint8_t Dato leído del registro.
     */
    uint8_t read_register_imu(uint8_t reg);




#endif