/**
 * @file LDC.h
 * 
 * @brief Archivo con la definición de funciones para el manejo del driver LDC.
 * 
 * Este archivo contiene la definición de funciones para el manejo del driver LDC, funciones
 * para la escritura de datos, el envio de comandos y la inicialización del display, el cual
 * es controlado mediante el circuito integrado GC9A01A y se comunica mediante el protocolo SPI.  
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see LDC.c
 * @see spi_driver.h
 * 
 * @date 21/11/2024
 * 
 * @version 1.0
 */

#ifndef LDC_H
#define LDC_H

#include "pico/stdlib.h"
#include "../drivers/spi_driver.h"


/**
 * @addtogroup LDC_DIMENSIONS
 * 
 * @{
 * 
 * Dimensiones del display LDC.
 * 
 */
/*! @brief Ancho del display*/
#define LCD_WIDTH 240
/*! @brief Alto del display*/
#define LCD_HEIGHT 240

/**
 * @}
 */

/**
 * @addtogroup LDC_COLORS
 * 
 * @{
 * 
 * Colores del display LDC.
 */
/*! @brief Color blanco*/
#define WHITE      				0xFFFF
/*! @brief color negro */
#define BLACK					0x0000
/*! @brief color azul*/
#define BLUE 					0x001F
/*! @brief color azul-rojo*/
#define BRED 					0XF81F
/*! @brief color verde-rojo*/
#define GRED 					0XFFE0
/*! @brief color verde-azul*/
#define GBLUE					0X07FF
/*! @brief color rojo*/
#define RED  					0xF800
/*! @brief color magenta*/
#define MAGENTA					0xF81F
/*! @brief color verde*/
#define GREEN					0x07E0
/*! @brief color cyan*/
#define CYAN 					0x7FFF
/*! @brief color amarillo*/
#define YELLOW					0xFFE0
/*! @brief color cafe*/
#define BROWN					0XBC40
/*! @brief color azul-rojo*/
#define BRRED					0XFC07
/*! @brief color gris*/
#define GRAY 	        		0X8430
/*! @brief color azul oscuro*/
#define DARKBLUE	    		0X01CF
/*! @brief color azul clar*/
#define LIGHTBLUE	    		0X7D7C
/*! @brief color azul gris*/
#define GRAYBLUE     		 	0X5458
/*! @brief color verde brillante*/
#define LIGHTGREEN    			0X841F
/*! @brief color gris l*/
#define LGRAY 		    		0XC618
/*! @brief color azul'gris*/
#define LGRAYBLUE     			0XA651
/*! @brief otro azul*/
#define LBBLUE        			0X2B12

/**
 * @}
 */

/**
 * @brief Estructura que contiene las dimensiones del display LDC y la direccion de escaneo
 * usada para almacebnar las configuraciones del display.
 */
typedef struct 
{
    uint16_t WIDTH;
    uint16_t HEIGHT;
    uint16_t SCAN_DIR;
}ldc_t;

extern ldc_t LDC;



/**
 * @addtogroup LDC_FUNCTIONS
 * 
 * @{
 * 
 * Funciones del driver LDC.
 * 
 */



#endif