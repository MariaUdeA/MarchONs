#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "pico/platform.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"


#include "../include/hardware/LCD.h"
#include "../include/drivers/spi_driver.h"
#include "../include/hardware/imu.h"
#include "../include/hardware/ds1302.h"
#include "../include/drivers/i2c_driver.h"

#define PLL_SYS_KHZ (270 * 1000)

datetime_t t = {
    .year  = 1523,
    .month = 06, //june
    .day   = 05,
    .dotw  = 5, // 0 is Sunday, so 5 is Friday
    .hour  = 15,
    .min   = 45,
    .sec   = 06
};

int main()
{
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        PLL_SYS_KHZ * 1000,
        PLL_SYS_KHZ * 1000);
    stdio_init_all();
    sleep_ms(5000);
    printf("Starting...\n");
    LCD_init(HORIZONTAL);
    LCD_Clear(WHITE);
    LCD_Display((uint16_t *)imagen_1);

    set_pwm(100);
  
    QMI8658_init();
    uint32_t steps = 0;
    //El tiempo solo se configura si no ha empezado aún, si está detenido, no reconfigura el tiempo.
    DS1302_init(&t);
    datetime_t now;
    while (true)
    {
        GetDateTime(&now);
        print_datetime(&now);

        sleep_ms(1000); // 1 sec
        // leer los pasos
        /*steps = read_imu_stepS_count();

        printf("pasos: %d\n", steps);
        sleep_ms(1000);*/
    }
}
