#include "../../include/drivers/twire_driver.h"


void tw_resetPins() {
    gpio_set_dir_in_masked(TW_PIN_MASK);
}

void tw_end() {
    tw_resetPins();
}

void tw_beginTransmission(uint8_t command) {
    gpio_set_dir(RTC_CS_PIN,GPIO_OUT);
    gpio_put(RTC_CS_PIN,false); //Not enabled :)

    gpio_set_dir(RTC_SCK_PIN,GPIO_OUT);
    gpio_put(RTC_SCK_PIN,false); //clock low

    gpio_set_dir(RTC_IO_PIN,GPIO_OUT);

    gpio_put(RTC_CS_PIN,true);
    sleep_us(4); // tCC = 4us NO SE QUE SIGNIFICA ESTO???????

    tw_write(command, (command & READ_FLAG) == READ_FLAG);
}

void tw_endTransmission() {
    gpio_put(RTC_CS_PIN,false);
    sleep_us(4);  // tCWH = 4us
}

void tw_init(){
    gpio_init_mask(TW_PIN_MASK);
    gpio_pull_down(RTC_IO_PIN);
    tw_resetPins();
}

void tw_write(uint8_t value, bool isDataRequestCommand) {
    for (uint8_t bit = 0; bit < 8; bit++) {
        gpio_put(RTC_IO_PIN, value & 0x01);
        sleep_us(1); //tDC =200ns

        // clock up, data is read by DS1302
        gpio_put(RTC_SCK_PIN,true);
        sleep_us(1);   // tCH = 1000ns, tCDH = 800ns

        // for the last bit before a read
        // Set IO line for input before the clock down
        if (bit == 7 && isDataRequestCommand) {
            gpio_set_dir(RTC_IO_PIN, GPIO_IN);
        }

        gpio_put(RTC_SCK_PIN,false);
        sleep_us(1);  // tCL=1000ns, tCDD=800ns

        value >>= 1;
    }
}

uint8_t tw_read() {
    uint8_t value = 0;

    for (uint8_t bit = 0; bit < 8; bit++) {
        // first bit is present on io pin, so only clock the other
        // bits
        value |= gpio_get(RTC_IO_PIN) << bit;
        
        // Clock up, prepare for next
        gpio_put(RTC_SCK_PIN,true);
        sleep_us(1);

        // Clock down, value is ready after some time.
        gpio_put(RTC_SCK_PIN,false);
        sleep_us(1);        // tCL=1000ns, tCDD=800ns
        }

    return value;
}