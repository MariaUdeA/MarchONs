/**
 * @file spi_driver.c
 * 
 * @brief Archivo con la implementación de funciones para el manejo del driver SPI.
 * 
 * Este archivo contiene la implementación de funciones para el manejo del driver SPI, contiene
 * la inicialización del driver y funciones para la escritura de datos.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see spi_driver.h
 * 
 * @date 20/11/2024
 * 
 * @version 1.0
 */





//Incluir header
#include "../../include/drivers/spi_driver.h"


void SPI_init(void){
    spi_init(SPI_PORT, SPI_FREQ);

    // Configura los pines de la interfaz SPI
    gpio_set_function(LDC_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LDC_MOSI_PIN, GPIO_FUNC_SPI);


    // Configura los pines de control del display
    gpio_init_mask(LDC_PIN_MASKS);
    gpio_set_dir_out_masked(LDC_PIN_MASKS);


    // Configurar el modo 0 del SPI
    spi_set_format(SPI_PORT, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

}

//wrapper write byte
void SPI_WriteByte(uint8_t Value)
{
    spi_write_blocking(SPI_PORT, &Value, 1);
}

//wrapper write string
void SPI_Write_nByte(uint8_t pData[], uint32_t Len)
{
    spi_write_blocking(SPI_PORT, pData, Len);
}