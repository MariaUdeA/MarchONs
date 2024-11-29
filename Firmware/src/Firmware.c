#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "pico/platform.h"

#include "hardware/i2c.h"
#include "hardware/gpio.h"

#include "../include/hardware/imu.h"
#include "../include/hardware/ds1302.h"
#include "../include/drivers/i2c_driver.h"

int main()
{
    stdio_init_all();
    sleep_ms(5000);

    QMI8658_init();
    uint32_t steps =0;

    while (true)
    {
        // leer los pasos
        steps = read_imu_step_count();

        printf("pasos: %d\n", steps);
        sleep_ms(1000);
    }
}
