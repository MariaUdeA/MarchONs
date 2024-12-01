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
dma_channel_config c;
uint dma_tx;
uint slice_num;

void config_display_pins(void) {
    // Configura los pines de control del display
    gpio_init(LCD_DC_PIN);
    gpio_init(LCD_CS_PIN);
    gpio_init(LCD_RESET_PIN);
    gpio_init(LCD_BL_PIN);

    // Configura los pines de control del display
    gpio_set_dir(LCD_DC_PIN, GPIO_OUT);
    gpio_set_dir(LCD_CS_PIN, GPIO_OUT);
    gpio_set_dir(LCD_RESET_PIN, GPIO_OUT);
    gpio_set_dir(LCD_BL_PIN, GPIO_OUT);


    // Configura el pin de selección del display
    gpio_put(LCD_CS_PIN, 1);
    gpio_put(LCD_DC_PIN, 0);
    gpio_put(LCD_BL_PIN, 1);
}

void config_RTC_pins(void) {
    // Configura los pines de control del RTC
    gpio_init(RTC_CS_PIN);
    gpio_set_dir(RTC_CS_PIN, GPIO_OUT);

    // Configura el pin de selección del RTC
    // Este CS es alto en 1
    gpio_put(RTC_CS_PIN, 0);
}

void config_pwm(void) {
    // Configura el PWM para controlar el brillo de la pantalla
    gpio_set_function(LCD_BL_PIN, GPIO_FUNC_PWM);
    slice_num = pwm_gpio_to_slice_num(LCD_BL_PIN);

    pwm_set_wrap(slice_num, 100);
    pwm_set_chan_level(slice_num, PWM_CHAN_B, 0);
    pwm_set_clkdiv(slice_num, 50);
    pwm_set_enabled(slice_num, true);
    

}
void config_dma(void) {
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
    channel_config_set_dreq(&c, spi_get_dreq(SPI_PORT, true));

}

void SPI_init(void){

    // Configura los pines de la interfaz SPI
    config_display_pins();

    // Configura el pwm para el control del brillo
    config_pwm();

    // Config
    spi_init(SPI_PORT, SPI_FREQ);

    // Configura los pines de la interfaz SPI
    gpio_set_function(LCD_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(LCD_MOSI_PIN, GPIO_FUNC_SPI);

    // Configura el dma
    config_dma();

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

void set_pwm(uint8_t level) {
    pwm_set_chan_level(slice_num, PWM_CHAN_B, level);
}

void SPI0_init(void){
    //Configuración del otro canal de SPI
    config_RTC_pins();
    //No creo que se necesite esto, pero lo dejo redactado por si acaso.
    spi_init(SPI_PORT0, SPI0_FREQ);
    // Configura los pines de la interfaz SPI
    gpio_set_function(RTC_SCK_PIN, GPIO_FUNC_SPI);
    gpio_set_function(RTC_MOSI_PIN, GPIO_FUNC_SPI);
    gpio_set_function(RTC_MISO_PIN, GPIO_FUNC_SPI);

}

void SPI0_WriteByte(uint8_t Value)
{
    uint8_t out=Value;
    if(SPI0_LSB_FIRST) out=swap_8[out];
    spi_write_blocking(SPI_PORT0, &out, 1);
}

uint8_t SPI0_ReadByte()
{
    uint8_t dst;
    spi_read_blocking(SPI_PORT0,0,&dst, 1);
    if(SPI0_LSB_FIRST){dst=swap_8[dst];}
    return dst;
}

void SPI0_beginTransmission(uint8_t command) {
    gpio_put(RTC_CS_PIN,true);
    sleep_us(4); // tCC = 4us NO SE QUE SIGNIFICA ESTO???????

    SPI0_WriteByte(command);
}

void SPI0_endTransmission() {
    gpio_put(RTC_CS_PIN,false);
    sleep_us(4);  // tCWH = 4us
}