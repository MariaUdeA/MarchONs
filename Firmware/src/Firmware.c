#include <stdio.h>
#include "pico/stdlib.h"
#include "../include/drivers/i2c_driver.h"
#include "../include/drivers/spi_driver.h"



int main()
{
    stdio_init_all();

    while (true) {
        
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
