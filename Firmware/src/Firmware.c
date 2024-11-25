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
#include "../include/drivers/i2c_driver.h"

#define PLL_SYS_KHZ (270 * 1000)




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
    LCD_init(HORIZONTAL);
    LCD_Clear(WHITE);
    //LCD_Display((uint16_t *)imagen_1);

    set_pwm(100);
  
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
