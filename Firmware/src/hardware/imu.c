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

void tap_read(void)
{
    uint8_t status = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STATUS1);
    printf("Status: 0x%02X\n", status);
    uint8_t mask = 0x01;
    if (status & mask)
    {
        printf("Tap detected\n");
    }
}

void QMI8658_enable_interrupt(void)
{
    // primero leer el registro de control 1
    uint8_t ctrl1 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);

    // habilitar las interrupciones en modo push pull
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL1, ctrl1 | CTRL1_INT_EN);
    printf("Interrupciones habilitadas\n");
    ctrl1 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);
}

void Non_SyncSample_mode(void)
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

    // desabilita el ctrl7
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, CTRL7_DISABLE_ALL);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, CTRL7_DISABLE_ALL);

    // verifica que todo el registro este en 0
    if (i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7) != 0x00)
    {
        printf("accel disble error\n");
        return;
    }
    printf("Accel disable and Non-SincSample mode\n");

    // habilitar las interrupciones en modo push pull
    uint8_t reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL1, CTRL1_INT_EN | reg_value);
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);
    printf("Interruptions enabled on CTRL1\n");

    printf("Configuring pedometer parameters...\n");

    // CONFIGURAR EL PEDOMETRO
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_L, PED_SAMPLE_CNT_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, PED_SAMPLE_CNT_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_L, PED_FIX_PEAK2PEAK_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_H, PED_FIX_PEAK2PEAK_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_L, PED_FIX_PEAK_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_H, PED_FIX_PEAK_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, 0x01);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_PEDOMETER);

    sleep_ms(10);
    if (i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STATUSINT) != STATUSINT_CMD_DONE)
    {
        printf("Error configuring pedometer 1\n");
        return;
    }

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_ACK);
    printf("Pedometer parameters 1 configured and ACK wrote on CTRL9\n");

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_L, PED_TIME_UP_L);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, PED_TIME_UP_H);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_L, PED_TIME_LOW);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL2_H, PED_CNT_ENTRY);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_L, PED_FIX_PRECISION);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL3_H, PED_SIG_COUNT);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL4_H, 0x02);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_PEDOMETER);

    sleep_ms(10);
    if (i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STATUSINT) != STATUSINT_CMD_DONE)
    {
        printf("Error configuring pedometer 1\n");
        return;
    }

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_ACK);
    printf("Pedometer parameters 2 configured and ACK wrote on CTRL9\n");

    // configurar el odr
    reg_value  = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL2);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL2, ODR_GYRO_56|reg_value|aFS_2g);
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL2);
    printf("ODR: 0x%02X\n", reg_value);

    // habilitar el pedometer
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, reg_value | CTRL7_ENABLE_ACC);

    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL8);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, reg_value | PEDOMETER_EN);

    uint8_t ctrl7 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7);
    uint8_t ctrl8 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL8);

    printf("CTRL7: 0x%02X, CTRL8: 0x%02X\n", ctrl7, ctrl8);

    printf("Pedometer enabled\n");
    

    printf("ODR\n");
    uint8_t ctrl5 = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL5);
    printf("CTRL5 (ODR): 0x%02X\n", ctrl5);

}
