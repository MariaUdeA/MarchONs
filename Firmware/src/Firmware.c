#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pll.h"
#include "hardware/clocks.h"
#include "hardware/sync.h"
#include "pico/platform.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

//Hardware libraries
#include "../include/hardware/LCD.h"
#include "../include/hardware/imu.h"
#include "../include/hardware/ds1302.h"
#include "../include/hardware/max30102.h"
#include "../include/pulse_read.h"

//Comunications drivers
#include "../include/drivers/i2c_driver.h"
#include "../include/drivers/spi_driver.h"
#include "../include/lib.h"

int main()
{
  smartwatch_init();
  return 0;
  
}
