/**
 * @file LCD.c
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
 * @see LCD.h
 * @see spi_driver.h
 * 
 * @date 21/11/2024
 * 
 * @version 1.0
 */

#include "stdio.h"
#include "../../include/hardware/LCD.h"
#include "../../include/drivers/spi_driver.h"

lcd_t LCD;

/**
 * @brief envia un comando al puerto spi1
 * 
 * @param Reg Comando a enviar
 */
static void LCD_SendCommand (uint8_t Reg) {

    gpio_put(LCD_DC_PIN, 0);

    SPI_WriteByte(Reg);


}

/**
 * @brief envia un dato al puerto spi1
 * 
 * @param Data Dato a enviar
 */
static void LCD_SendData_8bit (uint8_t Data) {
    gpio_put(LCD_DC_PIN, 1);
    SPI_WriteByte(Data);
}

/**
 * @brief envia un dato  de 16 bits al puerto spi1
 * 
 * @param Data Dato a enviar
 */
static void LCD_SendData_16bit (uint16_t Data) {
    gpio_put(LCD_DC_PIN, 1);
    SPI_WriteByte(Data >> 8);
    SPI_WriteByte(Data);
}


/**
 * @addtogroup Funciones de inicializacion
 * 
 * @{
 * 
 * Funciones usadas en la @see LCD_init()
 */


/**
 * @brief Realiza el reset de hardware al display
 */
static void LCD_reset (void) {
    gpio_put(LCD_RESET_PIN, 1);
    sleep_ms(100);
    gpio_put(LCD_RESET_PIN, 0);
    sleep_ms(100);
    gpio_put(LCD_RESET_PIN, 1);
    gpio_put(LCD_CS_PIN, 0);
    sleep_ms(100);

    printf("LCD Reset\n");
}


/**
 * @brief Inicializa los registros del display
 * 
 * Probablemente la funcion mas magica del planeta, solo la
 * persona que la escribio sabe que hace, magia pura.
 */
static void LCD_InitReg(void)
{
    LCD_SendCommand(0xEF);
	LCD_SendCommand(0xEB);
	LCD_SendData_8bit(0x14); 
	
    LCD_SendCommand(0xFE);			 
	LCD_SendCommand(0xEF); 

	LCD_SendCommand(0xEB);	
	LCD_SendData_8bit(0x14); 

	LCD_SendCommand(0x84);			
	LCD_SendData_8bit(0x40); 

	LCD_SendCommand(0x85);			
	LCD_SendData_8bit(0xFF); 

	LCD_SendCommand(0x86);			
	LCD_SendData_8bit(0xFF); 

	LCD_SendCommand(0x87);			
	LCD_SendData_8bit(0xFF);

	LCD_SendCommand(0x88);			
	LCD_SendData_8bit(0x0A);

	LCD_SendCommand(0x89);			
	LCD_SendData_8bit(0x21); 

	LCD_SendCommand(0x8A);			
	LCD_SendData_8bit(0x00); 

	LCD_SendCommand(0x8B);			
	LCD_SendData_8bit(0x80); 

	LCD_SendCommand(0x8C);			
	LCD_SendData_8bit(0x01); 

	LCD_SendCommand(0x8D);			
	LCD_SendData_8bit(0x01); 

	LCD_SendCommand(0x8E);			
	LCD_SendData_8bit(0xFF); 

	LCD_SendCommand(0x8F);			
	LCD_SendData_8bit(0xFF); 


	LCD_SendCommand(0xB6);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x20);

	LCD_SendCommand(0x36);
	LCD_SendData_8bit(0x08);//Set as vertical screen

	LCD_SendCommand(0x3A);			
	LCD_SendData_8bit(0x05); 


	LCD_SendCommand(0x90);			
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x08); 

	LCD_SendCommand(0xBD);			
	LCD_SendData_8bit(0x06);
	
	LCD_SendCommand(0xBC);			
	LCD_SendData_8bit(0x00);	

	LCD_SendCommand(0xFF);			
	LCD_SendData_8bit(0x60);
	LCD_SendData_8bit(0x01);
	LCD_SendData_8bit(0x04);

	LCD_SendCommand(0xC3);			
	LCD_SendData_8bit(0x13);
	LCD_SendCommand(0xC4);			
	LCD_SendData_8bit(0x13);

	LCD_SendCommand(0xC9);			
	LCD_SendData_8bit(0x22);

	LCD_SendCommand(0xBE);			
	LCD_SendData_8bit(0x11); 

	LCD_SendCommand(0xE1);			
	LCD_SendData_8bit(0x10);
	LCD_SendData_8bit(0x0E);

	LCD_SendCommand(0xDF);			
	LCD_SendData_8bit(0x21);
	LCD_SendData_8bit(0x0c);
	LCD_SendData_8bit(0x02);

	LCD_SendCommand(0xF0);   
	LCD_SendData_8bit(0x45);
	LCD_SendData_8bit(0x09);
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x26);
 	LCD_SendData_8bit(0x2A);

 	LCD_SendCommand(0xF1);    
 	LCD_SendData_8bit(0x43);
 	LCD_SendData_8bit(0x70);
 	LCD_SendData_8bit(0x72);
 	LCD_SendData_8bit(0x36);
 	LCD_SendData_8bit(0x37);  
 	LCD_SendData_8bit(0x6F);


 	LCD_SendCommand(0xF2);   
 	LCD_SendData_8bit(0x45);
 	LCD_SendData_8bit(0x09);
 	LCD_SendData_8bit(0x08);
 	LCD_SendData_8bit(0x08);
 	LCD_SendData_8bit(0x26);
 	LCD_SendData_8bit(0x2A);

 	LCD_SendCommand(0xF3);   
 	LCD_SendData_8bit(0x43);
 	LCD_SendData_8bit(0x70);
 	LCD_SendData_8bit(0x72);
 	LCD_SendData_8bit(0x36);
 	LCD_SendData_8bit(0x37); 
 	LCD_SendData_8bit(0x6F);

	LCD_SendCommand(0xED);	
	LCD_SendData_8bit(0x1B); 
	LCD_SendData_8bit(0x0B); 

	LCD_SendCommand(0xAE);			
	LCD_SendData_8bit(0x77);
	
	LCD_SendCommand(0xCD);			
	LCD_SendData_8bit(0x63);		


	LCD_SendCommand(0x70);			
	LCD_SendData_8bit(0x07);
	LCD_SendData_8bit(0x07);
	LCD_SendData_8bit(0x04);
	LCD_SendData_8bit(0x0E); 
	LCD_SendData_8bit(0x0F); 
	LCD_SendData_8bit(0x09);
	LCD_SendData_8bit(0x07);
	LCD_SendData_8bit(0x08);
	LCD_SendData_8bit(0x03);

	LCD_SendCommand(0xE8);			
	LCD_SendData_8bit(0x34);

	LCD_SendCommand(0x62);			
	LCD_SendData_8bit(0x18);
	LCD_SendData_8bit(0x0D);
	LCD_SendData_8bit(0x71);
	LCD_SendData_8bit(0xED);
	LCD_SendData_8bit(0x70); 
	LCD_SendData_8bit(0x70);
	LCD_SendData_8bit(0x18);
	LCD_SendData_8bit(0x0F);
	LCD_SendData_8bit(0x71);
	LCD_SendData_8bit(0xEF);
	LCD_SendData_8bit(0x70); 
	LCD_SendData_8bit(0x70);

	LCD_SendCommand(0x63);			
	LCD_SendData_8bit(0x18);
	LCD_SendData_8bit(0x11);
	LCD_SendData_8bit(0x71);
	LCD_SendData_8bit(0xF1);
	LCD_SendData_8bit(0x70); 
	LCD_SendData_8bit(0x70);
	LCD_SendData_8bit(0x18);
	LCD_SendData_8bit(0x13);
	LCD_SendData_8bit(0x71);
	LCD_SendData_8bit(0xF3);
	LCD_SendData_8bit(0x70); 
	LCD_SendData_8bit(0x70);

	LCD_SendCommand(0x64);			
	LCD_SendData_8bit(0x28);
	LCD_SendData_8bit(0x29);
	LCD_SendData_8bit(0xF1);
	LCD_SendData_8bit(0x01);
	LCD_SendData_8bit(0xF1);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x07);

	LCD_SendCommand(0x66);			
	LCD_SendData_8bit(0x3C);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0xCD);
	LCD_SendData_8bit(0x67);
	LCD_SendData_8bit(0x45);
	LCD_SendData_8bit(0x45);
	LCD_SendData_8bit(0x10);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x00);

	LCD_SendCommand(0x67);			
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x3C);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x00);
	LCD_SendData_8bit(0x01);
	LCD_SendData_8bit(0x54);
	LCD_SendData_8bit(0x10);
	LCD_SendData_8bit(0x32);
	LCD_SendData_8bit(0x98);

	LCD_SendCommand(0x74);			
	LCD_SendData_8bit(0x10);	
	LCD_SendData_8bit(0x85);	
	LCD_SendData_8bit(0x80);
	LCD_SendData_8bit(0x00); 
	LCD_SendData_8bit(0x00); 
	LCD_SendData_8bit(0x4E);
	LCD_SendData_8bit(0x00);					
	
    LCD_SendCommand(0x98);			
	LCD_SendData_8bit(0x3e);
	LCD_SendData_8bit(0x07);

	LCD_SendCommand(0x35);	
	LCD_SendCommand(0x21);

	LCD_SendCommand(0x11);
	sleep_ms(120);
	LCD_SendCommand(0x29);
	sleep_ms(20);

}




static void LCD_SetAttributes (uint8_t scan_dir) {


    LCD.SCAN_DIR = scan_dir;
    uint8_t MemoryAccess = 0x08;


    if (scan_dir == HORIZONTAL) {
        LCD.WIDTH = LCD_WIDTH;
        LCD.HEIGHT = LCD_HEIGHT;
        MemoryAccess = 0xc8;
    } else {
        LCD.WIDTH = LCD_HEIGHT;
        LCD.HEIGHT = LCD_WIDTH;
        MemoryAccess = 0x68;
    }


    LCD_SendCommand(0x36);

    LCD_SendData_8bit(MemoryAccess);     

}

/**
 * @}
 */

void LCD_1IN28_SetWindows(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
    // Set the X coordinates
    LCD_SendCommand(0x2A);
    LCD_SendData_8bit(0x00);
    LCD_SendData_8bit(Xstart);
    LCD_SendData_8bit((Xend)>>8);
    LCD_SendData_8bit(Xend);

    // Set the Y coordinates
    LCD_SendCommand(0x2B);
    LCD_SendData_8bit(0x00);
    LCD_SendData_8bit(Ystart);
    LCD_SendData_8bit((Xend)>>8);
    LCD_SendData_8bit(Yend);

    LCD_SendCommand(0x2C);
    gpio_put(LCD_DC_PIN, 1);
}

void LCD_Clear(uint16_t color) {

    uint16_t j;
    uint16_t image[LCD_WIDTH* LCD_HEIGHT];

    color = ((color << 8)& 0xFF00) | (color >> 8);

    for (j = 0; j < 28800; j++) {
        image[j] = color;
    }
    color = BLUE;
    color = ((color << 8)& 0xFF00) | (color >> 8);


    for (j = 28800; j < 57600; j++)
    {
        image[j] = color;
    }
    

    LCD_1IN28_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    gpio_put(LCD_DC_PIN, 1);

    for (j = 0; j < LCD_HEIGHT; j++) {
        SPI_Write_nByte((uint8_t *)&image[j * LCD_WIDTH], LCD_WIDTH* 2);
    }

}   

void LCD_Display (uint16_t *image)
{
    uint16_t j;
    LCD_1IN28_SetWindows(0, 0, LCD_WIDTH, LCD_HEIGHT);
    gpio_put(LCD_DC_PIN, 1);

    for (j = 0; j < LCD_HEIGHT; j++) {
        SPI_Write_nByte((uint8_t *)&image[j * LCD_WIDTH], LCD_WIDTH* 2);
    }

}

void LCD_1IN28_DisplayWindows (uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image) {

    uint32_t Addr = 0;
    uint16_t j;

    LCD_1IN28_SetWindows(Xstart, Ystart, Xend, Yend);
    gpio_put(LCD_DC_PIN, 1);

    for (j = Ystart; j < Yend; j++) {
        Addr = j * LCD_WIDTH + Xstart;
        SPI_Write_nByte((uint8_t *)&Image[Addr], (Xend - Xstart) * 2);
    }

}


void LCD_init (uint8_t scan_dir) {
    // Reseteo por medio del pin

    SPI_init();

    LCD_reset();

    // Inicializar los atributos de la estructura LDC

    LCD_SetAttributes(scan_dir);


    // Inicializar el display mediante los comandos

    LCD_InitReg();

    printf("LCD Init success\n");
} 



