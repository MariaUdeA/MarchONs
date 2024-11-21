/**
 * @file LDC.c
 * 
 * @brief Archivo con la implementación de funciones para el manejo del driver LDC.
 * 
 * Este archivo contiene la implementación de funciones para el manejo del driver LDC, funciones
 * para la escritura de datos, el envio de comandos y la inicialización del display, el cual
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see LDC.h
 * @see spi_driver.h
 * 
 * @date 21/11/2024
 * 
 * @version 1.0
 */

#include "../include/hardware/LDC.h"
#include "../include/drivers/spi_driver.h"

void LDC_send_command(uint8_t command)
{
    gpio_put(LDC_DC_PIN, 0);
    spi_write_command(command);

}

void LDC_send_data(uint8_t data)
{
    gpio_put(LDC_DC_PIN, 1);
    spi_write_data(data);
}

