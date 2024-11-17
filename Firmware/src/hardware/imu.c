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
 *
 **/

#include "stdio.h"
#include "../../include/hardware/imu.h"
#include "../../include/drivers/i2c_driver.h"
static uint8_t qmi8658_who_am_i;
static uint8_t qmi8658_reset_status;


void tap_read (void) 
{
    uint8_t status = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STATUS1);
    printf("Status: 0x%02X\n", status);
    uint8_t mask = 0x01;
    if (status & mask)
    {
        printf("Tap detected\n");
    }
}

void tap_config (void)
{
    // desactivar ctrl7 Y entra en modo non syncsample
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, CTRL7_DISABLE_ALL);


    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL2, ODR_ACC_448 | aFS_16g | DIS_SELF_TEST);
    
    // Escribir el primer set de configuraciones
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_L, PEAK_WINDOW);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, PRIORITY);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_L, TAP_WINDOW_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_H, TAP_WINDOW_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_L, DTAP_WINDOW_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_H, DTAP_WINDOW_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, 0x10|TAP_FIRST_COMMAND);     // inicia el primer comando
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, CTRL_CMD_CONFIGURE_TAP);     // Aplica la configuracion


    // Escribir el segundo set de configuraciones
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_L, ALPHA);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, GAMMA);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_L, PEAK_MAG_THR_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_H, PEAK_MAG_THR_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_L, UDM_THR_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_H, UDM_THR_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, 0x10|TAP_SECOND_COMMAND);     // inicia el primer comando
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, CTRL_CMD_CONFIGURE_TAP);     // Aplica la configuracion

    // habilitar  el aceletometro
    uint8_t reg_value = (I2C_PORT, QMI8568A_ADDR, CTRL7);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, 0x01|reg_value);

    // habilitar las interrupciones y el tap
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL8);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, reg_value | CTRL8_INT1 | CTRL8_ENABLE_EVENTS);



}


void QMI8658_enable_interrupt (void) 
{
    //primero leer el registro de control 1
    uint8_t ctrl1 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);

    //habilitar las interrupciones en modo push pull
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL1, ctrl1 | CTRL1_INT_EN);
}

void Non_SyncSample_mode (void)
{
    uint8_t reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, reg_value | CTRL7_DISABLE_ALL);

    // Map the interrupt to INT1 and enable the events detection
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL8);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, reg_value | CTRL8_INT1 | CTRL8_ENABLE_EVENTS);

}



void QMI8658_init(void)
{
    // Iniciar el puerto I2C
    smartwatch_i2c_init();

    // Resetear el sensor
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, SOFT_RESET, RESET_IMU);

    
    qmi8658_reset_status = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, dQY_L);
    if (qmi8658_reset_status != QMI8658A_RESET_SUCCES)
    {
        printf("Error reseting QMI8568\n");
        printf("QMI8658A reset status: 0x%02X\n", qmi8658_reset_status);
        return;
    }
    printf("QMI8658A reseted\n");


    // Leer el WHO_AM_I
    qmi8658_who_am_i = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, WHO_AM_I);

    if (qmi8658_who_am_i != QMI8568A_WHO_AM_I)
    {
        printf("Error conection with QMI8568\n");
        return;
    }

    printf("Conection found with QMI8568\n");
    printf("QMI8658A sensor at: 0x%02X   WHO_AM_I: 0x%02X\n", QMI8568A_ADDR, QMI8568A_WHO_AM_I);

    
    tap_config();




}
