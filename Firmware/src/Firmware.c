#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "pico/platform.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"


#include "../include/hardware/imu.h"
#include "../include/drivers/i2c_driver.h"



int main()
{
    stdio_init_all();
    sleep_ms(5000);

    QMI8658_init();

    while (true) {
        sleep_ms(10);
        tap_read();  
    }
}
