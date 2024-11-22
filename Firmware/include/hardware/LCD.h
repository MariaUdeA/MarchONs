/**
 * @file LCD.h
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
 * @see LCD.c
 * @see spi_driver.h
 *
 * @date 21/11/2024
 *
 * @version 1.0
 */

#ifndef LCD_H
#define LCD_H

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
/*! @brief Direccion de escaneo horizontal*/
#define HORIZONTAL 0
/*! @brief Direccion de escaneo vertical*/
#define VERTICAL 1
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
#define WHITE 0xFFFF
/*! @brief color negro */
#define BLACK 0x0000
/*! @brief color azul*/
#define BLUE 0x001F
/*! @brief color azul-rojo*/
#define BRED 0XF81F
/*! @brief color verde-rojo*/
#define GRED 0XFFE0
/*! @brief color verde-azul*/
#define GBLUE 0X07FF
/*! @brief color rojo*/
#define RED 0xF800
/*! @brief color magenta*/
#define MAGENTA 0xF81F
/*! @brief color verde*/
#define GREEN 0x07E0
/*! @brief color cyan*/
#define CYAN 0x7FFF
/*! @brief color amarillo*/
#define YELLOW 0xFFE0
/*! @brief color cafe*/
#define BROWN 0XBC40
/*! @brief color azul-rojo*/
#define BRRED 0XFC07
/*! @brief color gris*/
#define GRAY 0X8430
/*! @brief color azul oscuro*/
#define DARKBLUE 0X01CF
/*! @brief color azul clar*/
#define LIGHTBLUE 0X7D7C
/*! @brief color azul gris*/
#define GRAYBLUE 0X5458
/*! @brief color verde brillante*/
#define LIGHTGREEN 0X841F
/*! @brief color gris l*/
#define LGRAY 0XC618
/*! @brief color azul'gris*/
#define LGRAYBLUE 0XA651
/*! @brief otro azul*/
#define LBBLUE 0X2B12

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
    uint8_t SCAN_DIR;
} lcd_t;

extern lcd_t LCD;

/**
 * @addtogroup LDC_FUNCTIONS
 *
 * @{
 *
 * Funciones del driver LDC.
 *
 */

/**
 * @brief Función que inicializa el driver LDC.
 *
 * Esta función inicializa el driver LDC, configurando los registros necesarios para su correcto funcionamiento.
 *
 * @param scan_dir Direccion de escaneo del display.
 *
 * @return void
 */
void LCD_init(uint8_t scan_dir);

/**
 * @brief Función que configura las dimensiones del display.
 *
 * Esta función configura las dimensiones del display, estableciendo el ancho y el alto del display.
 * o la region donde se quiere dibujar
 *
 * @param Xstart Coordenada X de inicio.
 * @param Ystart Coordenada Y de inicio.
 * @param Xend Coordenada X de fin.
 * @param Yend Coordenada Y de fin.
 *
 */
void LCD_1IN28_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);

/**
 * @brief Función que limpia la pantalla del display.
 *
 * Esta función limpia la pantalla del display, pintando toda la pantalla de un color especifico.
 *
 * @param color Color con el que se va a pintar la pantalla.
 *
 */
void LCD_Clear(uint16_t color);

/**
 * @brief Función que muestra una imagen en el display.
 *
 * Esta función muestra una imagen en el display, la imagen debe ser un arreglo de 16 bits.
 *
 * @param image Arreglo de 16 bits que contiene la imagen.
 */
void LCD_Display(uint16_t *image);


/**
 * @brief Función que muestra una imagen en el display.
 *
 * Esta función muestra una imagen en el display, la imagen debe ser un arreglo de 16 bits.
 *
 * @param Xstart Coordenada X de inicio.
 * @param Ystart Coordenada Y de inicio.
 * @param Xend Coordenada X de fin.
 * @param Yend Coordenada Y de fin.
 * @param Image Arreglo de 16 bits que contiene la imagen.
 */
void LCD_1IN28_DisplayWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image);

	


	
#endif