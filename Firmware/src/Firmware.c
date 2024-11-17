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
        uint8_t steps = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_LOW);
        printf("Steps: %d\n", steps);

        steps = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_MIDL);
        printf("Steps: %d\n", steps);

        steps = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_HIGH);
        printf("Steps: %d\n", steps);
        

        sleep_ms(1000);
    }
}
