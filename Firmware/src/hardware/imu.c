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

static void disable_ctrl7(void)
{
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, CTRL7_DISABLE_ALL);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, CTRL7_DISABLE_ALL);
}

static void set_ODR()
{
    uint8_t reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL2);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL2, ODR_GYRO_56|reg_value|aFS_2g);
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL2);
    printf("ODR: 0x%02X\n", reg_value);

}


uint32_t read_imu_step_count(void)
{
    uint32_t steps = 0;
    uint8_t step_low = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_LOW);
    uint8_t step_mid = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_MIDL);
    uint8_t step_high = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STEP_CNT_HIGH);

    steps = (step_high << 16) | (step_mid << 8) | step_low;

    return steps;
}

int reset_imu (void)
{
    // Resetear el sensor
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, SOFT_RESET, RESET_IMU);

    qmi8658_reset_status = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, dQY_L);
    if (qmi8658_reset_status != QMI8658A_RESET_SUCCES)
    {
        printf("QMI8658A reset failed\n");
        printf("QMI8658A reset status: 0x%02X\n", qmi8658_reset_status);
        return -1;
    }
    printf("QMI8658A reseted\n");

    return 0;
}

void config_interrupts(void)
{
    uint8_t reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL1, CTRL1_INT_EN | reg_value);
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL1);
    printf("Interruptions enabled on CTRL1\n");

}


int imu_config_pedometer_params (void)
{

    // verifica que todo el registro este en 0
    if (i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7) != 0x00)
    {
        printf("accel disble error\n");
        return -1;
    }
    printf("Accel disable and Non-SincSample mode\n");


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
        return -1;
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
        return -1;
    }

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_ACK);
    printf("Pedometer parameters 2 configured and ACK wrote on CTRL9\n");

    return 0;
}

int imu_config_WoM_params (void) {
    // configurar el WoM
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_L, QMI8658WoMThreshold_high);
    sleep_ms(10);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, QMI8658WoMiNT1Config);
    sleep_ms(10);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CAL1_H, QMI8658WoMBlankingTime);
    sleep_ms(10);

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_WRITE_WOM_SETTING);

    if (i2c_read_byte(I2C_PORT, QMI8568A_ADDR, STATUSINT) != STATUSINT_CMD_DONE)
    {
        printf("Error configuring WoM\n");
        return -1;
    }

    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL9, CTRL_CMD_ACK);
    printf("WoM configured and ACK wrote on CTRL9\n");

    return 0;

}


void enable_motion_events(void)
{
    // habilitar el acelerometro   
    uint8_t reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL7);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL7, reg_value | CTRL7_ENABLE_ACC);

    // habilitar el pedometer
    reg_value = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, CTRL8);
    i2c_write_byte(I2C_PORT, QMI8568A_ADDR, CTRL8, reg_value | PEDOMETER_EN);

}


void QMI8658_init (void)
{
    // Iniciar el puerto I2C
    smartwatch_i2c_init();

    // Resetear el sensor
    if (reset_imu() != 0) {return;}


    // Leer el WHO_AM_I
    qmi8658_who_am_i = i2c_read_byte(I2C_PORT, QMI8568A_ADDR, WHO_AM_I);

    if (qmi8658_who_am_i != QMI8568A_WHO_AM_I)
    {
        printf("Error conection with QMI8568\n");
        return;
    }

    printf("Conection found with QMI8568\n");
    printf("QMI8658A sensor at: 0x%02X   WHO_AM_I: 0x%02X\n", QMI8568A_ADDR, QMI8568A_WHO_AM_I);

    // desabilitar el CTRL7, necesario para ambas configuraciones
    disable_ctrl7();

    // Configurar ODR y full-scale del acelerometro
    set_ODR();

    // habilitar las interrupciones en modo push pull
    config_interrupts();


    // configurar los parametros del pedometer

    printf("Configuring pedometer parameters...\n");
    if (imu_config_pedometer_params() != 0) {return;}
    printf("Pedometer parameters configured\n");

    // configurar los parametros del WoM
    printf("Configuring WoM parameters...\n");
    if (imu_config_WoM_params() != 0) {return;}
    printf("WoM parameters configured\n");


    // habilitar el pedometer
    enable_motion_events();

    printf("motion events enable\n");
}

