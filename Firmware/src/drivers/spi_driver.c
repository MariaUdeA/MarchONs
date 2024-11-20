//Incluir header
#include "../../include/drivers/spi_driver.h"

//Inicializar el spi
void SPI_init(void){
    //check datasheet to see which connection
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