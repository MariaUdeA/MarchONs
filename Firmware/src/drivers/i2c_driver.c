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


#include "stdio.h"
#include "../../include/drivers/i2c_driver.h"


/********************************************************************************************************************************************
 * 
 * funciones para la lectura y escritura de datos en el bus I2C
 * ******************************************************************************************************************************************
*/

void i2c_write_byte (i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t value) 
{
    uint8_t data[2] = {reg, value};
    i2c_write_blocking(i2c_port, addr, data, 2, false);

}

void i2c_write_nbytes(i2c_inst_t *i2c_port, uint8_t addr, uint8_t *pData, uint32_t len)
{
    i2c_write_blocking(i2c_port, addr, pData, len, false);
}

uint8_t i2c_read_byte(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg)
{
    uint8_t buf;

    // Write the register to read
    i2c_write_blocking(i2c_port, addr, &reg, 1, true);
    // Read the register
    i2c_read_blocking(i2c_port, addr, &buf, 1, false);

    // printf("Read byte: 0x%02X on register 0x%02X\n", buf, reg);
    return buf;
}

void i2c_read_nbytes(i2c_inst_t *i2c_port, uint8_t addr, uint8_t reg, uint8_t *pData, uint32_t len)
{
    // Write the register to read
    i2c_write_blocking(i2c_port, addr, &reg, 1, true);
    // Read the register
    i2c_read_blocking(i2c_port, addr, pData, len, false);
}



/********************************************************************************************************************************************
 * 
 * funciones para la configuración del sensor IMU
 * ******************************************************************************************************************************************
*/


void imu_pin_setup(void) 
{
    // configurar los pines de interrupción
    gpio_init(DOF_INT1);
    gpio_set_dir(DOF_INT1, GPIO_IN);

    gpio_init(DOF_INT2);
    gpio_set_dir(DOF_INT2, GPIO_IN);


    // configurar la interrupción
    // gpio_set_irq_enabled_with_callback(DOF_INT1, GPIO_IRQ_LEVEL_LOW, true, &gpio_callback);
    // gpio_set_irq_enabled_with_callback(DOF_INT2, GPIO_IRQ_LEVEL_HIGH, true, &gpio_callback);
}

void gpio_callback(uint gpio, uint32_t events)
{
    printf("interrupt detected\n");
    // Aqui deberia cambiar alguna bandera dependiendo del pin que genere la interrupcion
}

/********************************************************************************************************************************************
 * 
 * funciones para la configuración del RTC
 * ******************************************************************************************************************************************
*/


void smartwatch_i2c_init(void)
{
    i2c_init(i2c1, I2C_FREQ);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);

    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Inicializar la IMU
    imu_pin_setup();


    // Inicializar el RTC
    //


} 