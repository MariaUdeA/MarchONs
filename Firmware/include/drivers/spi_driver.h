/**
 * @file spi_driver.h
 *
 * @brief Archivo con la definición de funciones para el manejo del driver SPI.
 *
 * Este archivo contiene la definición de funciones para el manejo del driver SPI, desde
 * la inicialización hasta la lectura y escritura de datos.
 *
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see spi_driver.c
 *
 * @date 20/11/2024
 *
 * @version 1.0
 */

#ifndef spi_driver_H

#define spi_driver_H

#include "pico/stdlib.h"
#include "hardware/spi.h"

/**
 * @addtogroup SPI_CONFIG
 *
 * @{
 *
 * Configuración del driver SPI.
 *
 */
/*! @brief Puerto spi*/
#define SPI_PORT spi1
/*! @brief Frecuencia de la comunicacion spi*/
#define SPI_FREQ (270000 * 1000)

/*! @brief LDC command/data selection pin*/
#define LDC_DC_PIN 8
/*! @brief LDC chip selection*/
#define LDC_CS_PIN 9
/*! @brief LDC SCK pin*/
#define LDC_SCK_PIN 10
/*! @brief LDC MOSI pin*/
#define LDC_MOSI_PIN 11
/*! @brief LDC RESET PIN*/
#define LDC_RESET_PIN 12
/*! @brief LDC BLACKLIGHT CONTROL pin*/
#define LDC_BL_PIN 25
/*! @brief Marcara de los pines de control del display*/
#define LDC_PIN_MASKS ((1U << LDC_DC_PIN) | (1U << LDC_CS_PIN) | (1U << LDC_RESET_PIN) | (1U << LDC_BL_PIN))

/**
 * @}
 */

/**
 * @addtogroup SPI_FUNCTIONS
 *
 * @{
 *
 * Funciones del driver SPI.
 */

/**
 * @brief Función que inicializa el driver SPI.
 *
 * Esta función inicializa el driver SPI, configurando los registros necesarios para su correcto funcionamiento.
 */
void SPI_init(void);


/**
 * @brief Función que envía un comando al display.
 *
 * Esta función envía un comando al display.
 *
 * @param command Comando a enviar.
 *
 * @return void
 */
void send_command(uint8_t command);




/**
 * @brief Función que escribe un byte en el puerto SPI.
 *
 * Esta función escribe un byte en el puerto SPI.
 *
 * @param Value Byte a escribir.
 *
 * @return void
 */
void SPI_WriteByte(uint8_t Value);

/**
 * @brief Función que escribe una cadena de bytes en el puerto SPI.
 *
 * Esta función escribe una cadena de bytes en el puerto SPI.
 *
 * @param pData Cadena de bytes a escribir.
 * @param Len Longitud de la cadena de bytes.
 *
 * @return void
 */
void SPI_Write_nByte(uint8_t pData[], uint32_t Len);

/**
 * @}
 */

#endif
