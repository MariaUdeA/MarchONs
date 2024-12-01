

#ifndef twire_driver_H

#define twire_driver_H

#include "stdio.h"
#include "pico/stdlib.h"

/*! @brief RTC chip selection*/
#define RTC_CS_PIN 17
/*! @brief RTC SCK pin*/
#define RTC_SCK_PIN 18
/*! @brief RTC MOSI pin*/
#define RTC_IO_PIN 16
/*! @brief Clock Period*/
#define PORT_US 10

/*! @brief Pin Mask for Three Wire*/
#define TW_PIN_MASK ((1U << RTC_CS_PIN) | (1U << RTC_IO_PIN) | (1U << RTC_SCK_PIN))



/*! @brief In bit*/
#define IO_IN 0
/*! @brief Out bit*/
#define IO_OUT 1

#define READ_FLAG 1

void tw_beginTransmission(uint8_t command);
void tw_endTransmission();


/**
 * @brief Función que inicializa la comunicación three wire.
 *
 * @return void
 */
void tw_init();

/**
 * @brief Función que escribe un byte en el pin IO.
 *
 * Esta función escribe un byte en el pin IO.
 *
 * @param Value Byte a escribir.
 *
 * @return void
 */
void tw_write(uint8_t value,bool isDataRequestCommand);

/**
 * @brief Función que lee un byte en el pin IO.
 *
 * Esta función lee un byte en pin IO.
 *
 * @param Value Byte a escribir.
 *
 * @return void
 */
uint8_t tw_read();



#endif