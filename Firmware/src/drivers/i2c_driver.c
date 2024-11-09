/**
 * @file i2c_driver.c
 * 
 * @brief Archivo con la implementación de funciones para el manejo del driver I2C.
 * 
 * Este archivo contiene la implementación de funciones para el manejo del driver I2C
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see i2c_driver.h
 * 
 * @date 08/11/2024
 * 
 * @version 1.0
 */

#include "../../include/drivers/i2c_driver.h"

/**
 * @brief Función que inicializa el driver I2C.
 * 
 * Esta función inicializa el driver I2C, configurando los registros necesarios para su correcto funcionamiento.
 * 
 * @return void
 */
void i2c_init_setup (void) {

    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

/**
 * @brief Función que escribe un registro en el sensor IMU.
 * 
 * Esta función escribe un registro del sensor IMU.
 * 
 * @param reg Registro a leer.
 * @param data Dato a escribir en el registro.
 * 
 */
void write_register_imu (uint8_t reg, uint8_t data) {
    uint8_t buffer[2] = {reg, data};
    i2c_write_blocking(I2C_PORT, IMU_ADDR, buffer, 2, false);
}

/**
 * @brief Función que lee un registro del sensor IMU.
 * 
 * Esta función lee un registro del sensor IMU.
 * 
 * @param reg Registro a leer.
 * 
 * @return uint8_t Dato leído del registro.
 */
uint8_t read_register_imu (uint8_t reg) {

    uint8_t data;
    
    i2c_write_blocking(I2C_PORT, IMU_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, IMU_ADDR, &data, 1, false);
    return data;
}