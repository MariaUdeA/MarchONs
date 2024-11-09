/**
 * @file imu.c
 * 
 * @brief Archivo con la implementación de funciones para el manejo del sensor IMU.
 * 
 * Este archivo contiene la implementación de funciones para el manejo del sensor IMU, desde
 * la inicialización hasta la lectura de datos.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see imu.h
 * 
 * @date 08/11/2024
 * 
 * @version 1.0
 * 
 */

#include "../../include/hardware/imu.h"
#include "../../include/drivers/i2c_driver.h"


int imu_who_am_i(void) {
    int who_am_i;

    uint8_t who_am_i_reg = WHO_AM_I;

    int result = i2c_write_blocking(I2C_PORT, IMU_ADDR, &who_am_i_reg, 1, true);

    if (result != PICO_ERROR_GENERIC) {

        uint8_t data;
        i2c_read_blocking(I2C_PORT, IMU_ADDR, &data, 1, false);
        who_am_i = data;

    } else {
        who_am_i = -1;
    }

    return who_am_i;
}

void configure_pedometer (void) {
    // Para mas información ver el datasheet del QMI8658A pg. 65
    
    // Escribir 0x01 a CAL4_H
    write_register_imu(CAL4_H, 0x01);

    // Escribir 0x02 en CAL4_H
    write_register_imu(CAL4_H, 0x02);

    // Escribir 0x0D en CTRL9
    write_register_imu(CTRL9, bCTRL_CMD_CONFIGURE_PEDOMETER);

}

void enable_pedometer (void) {
    
    // Habilitar el acelerómetro
    write_register_imu(CTRL7, (1 << bCTRL_ACCEL_EN));

    // Habilitar el pedometer
    write_register_imu(CTRL8, (1 << bCTRL_PEDOMETER_EN));
}

void configure_pedometer_interrupt (void) {

    // Configurar el pedometer para que genere una interrupción cuando se detecte un paso
    write_register_imu(CTRL8, (0x01 << 6));

    // Habilita la interrupcion en INT1
    write_register_imu(CTRL1, (0x01 << 3));
}

uint32_t imu_read_steps (void) {
    
    uint8_t step_cnt_l = read_register_imu(STEP_CNT_L);
    uint8_t step_cnt_m = read_register_imu(STEP_CNT_M);
    uint8_t step_cnt_h = read_register_imu(STEP_CNT_H);

    uint32_t steps = (step_cnt_h << 16) | (step_cnt_m << 8) | step_cnt_l;

    return steps;
}


void imu_init_setup(void) {
    i2c_init_setup();
    configure_pedometer();
    enable_pedometer();
    configure_pedometer_interrupt();
    
}