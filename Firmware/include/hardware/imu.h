/**
 * @file imu.h
 *
 * @brief Archivo con la definición de funciones para el manejo del sensor IMU.
 *
 * Este archivo contiene la definición de funciones para el manejo del sensor IMU, desde
 * la inicialización hasta la lectura de datos.
 *
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see imu.c
 *
 * @date 08/11/2024
 *
 * @version 1.0
 *
 */

#ifndef IMU_H

#define IMU_H

#include "pico/stdlib.h"

/*! @brief Dirección del sensor IMU QMI8658 */
#define QMI8568A_ADDR       0x6B
/*! @brief Valor por defecto del registro Who am I*/
#define QMI8568A_WHO_AM_I   0x05
/*! @brief Valor por defecto del registro Revision Id*/
#define QMI8568A_REVISION_ID 0x7C

/*! @brief Valor para resetear el sensor*/
#define RESET_IMU            0x0B
/*! @brief Valor para la verificacion del reset*/
#define QMI8658A_RESET_SUCCES 0x80

/*! @brief Mascara para escribir en el registro CTRL1 y habilitar las interupciones en modo push pull*/
#define CTRL1_INT_EN (0x03 << 3)
/*! @brief Mascara para desabilitar las configuraciones del registro CTRL7*/
#define CTRL7_DISABLE_ALL 0x0
/*! @brief Mascara para mapear la interrupcion al pin INT1*/
#define CTRL8_INT1 (0x01 << 6)
/*! @brief Mascara para habilitar los eventos en el registro CTRL8*/
#define CTRL8_ENABLE_EVENTS 0x19


/**
 * @addtogroup QMI8658A_TAP_PARAMS
 * 
 * @{
 * 
 * Parámetros para la configuración del tap del sensor IMU.
 */

// Priority: Define la prioridad de los ejes (X > Y > Z)
#define PRIORITY            0x00 // (X > Y > Z) según tabla del manual

// PeakWindow: Duración máxima para un pico válido (40 ms @ 500 Hz)
#define PEAK_WINDOW         20   // 40 ms / 2 ms por muestra

// TapWindow: Tiempo mínimo antes de un segundo toque (100 ms @ 500 Hz)
#define TAP_WINDOW_L        (100 / 2) & 0xFF       // Parte baja (50 muestras)
#define TAP_WINDOW_H        ((100 / 2) >> 8) & 0xFF // Parte alta

// DTapWindow: Tiempo máximo para un segundo toque (300 ms @ 500 Hz)
#define DTAP_WINDOW_L       (300 / 2) & 0xFF       // Parte baja (150 muestras)
#define DTAP_WINDOW_H       ((300 / 2) >> 8) & 0xFF // Parte alta

// Alpha: Relación para promedio de aceleración (0.0625)
#define ALPHA               ((uint8_t)(0.0625 / 0.0078)) // ≈ 8

// Gamma: Relación para promedio de magnitud (0.25)
#define GAMMA               ((uint8_t)(0.25 / 0.0078))   // ≈ 32

// PeakMagThr: Umbral para detección de pico (0.8 g²)
#define PEAK_MAG_THR_L      ((uint16_t)(0.8 / 0.01) & 0xFF)       // Parte baja
#define PEAK_MAG_THR_H      (((uint16_t)(0.8 / 0.01) >> 8) & 0xFF) // Parte alta

// UDMThr: Umbral para estado de reposo (0.4 g²)
#define UDM_THR_L           ((uint16_t)(0.4 / 0.001) & 0xFF)       // Parte baja
#define UDM_THR_H           (((uint16_t)(0.4 / 0.001) >> 8) & 0xFF) // Parte alta

// Comandos
#define TAP_FIRST_COMMAND   0x01
#define TAP_SECOND_COMMAND  0x02
#define CTRL_CMD_CONFIGURE_TAP  0x0C


/**
 * @}
 * 
 */





/**
 * @addtogroup IMU_REGISTERS
 * @{
 *
 * Direcciones de los registros de la IMU.
 */

enum IMU_REGISTERS
{
    /*! \brief Devise Identifier*/
    WHO_AM_I = 0x00,
    /*! \brief Device Register ID*/
    REVISION_ID = 0x01,
    /*! \brief SPI interface and sensor enable*/
    CTRL1 = 0x02,
    /*! \brief Accelerometer: OPutput Data Rate, Full Scale, Self test*/
    CTRL2 = 0x03,
    /*! \brief Gyroscope: Output Data Rate, Full Scale, Self test*/
    CTRL3 = 0x04,
    /*! \brief RESERVED*/
    CTRL4 = 0x05,
    /*! \brief Low pass filter setting*/
    CTRL5 = 0x06,
    /*! \brief RESERVED*/
    CTRL6 = 0x07,
    /*! \brief RESERVED 1*/
    CTRL7 = 0x08,
    /*! \brief Motion Detection Control*/
    CTRL8 = 0x09,
    /*! \brief Host commands*/
    CTRL9 = 0x0A,
    /*! \brief Calibration Register 1 lower bits*/
    CAL1_L = 0x0B,
    /*! \brief Calibration Register 1 higher bits*/
    CAL1_H = 0x0C,
    /*! \brief Calibration Register 2 lower bits*/
    CAL2_L = 0x0D,
    /*! \brief Calibration Register 2 higher bits*/
    CAL2_H = 0x0E,
    /*! \brief Calibration Register 3 lower bits*/
    CAL3_L = 0x0F,
    /*! \brief Calibration Register 3 higher bits*/
    CAL3_H = 0x10,
    /*! \brief Calibration Register 4 lower bits*/
    CAL4_L = 0x11,
    /*! \brief Calibration Register 4 higher bits*/
    CAL4_H = 0x12,
    /*! \brief FIFO watermark level in ODRs*/
    FIFO_WTH_TH = 0x13,
    /*! \brief FIFO control*/
    FIFO_CTRL = 0x14,
    /*! \brief FIFO Sample count LSBs*/
    FIFO_SMPL_CONT = 0x15,
    /*! \brief FIFO Status*/
    FIFO_STATUS = 0x16,
    /*! \brief FIFO data*/
    FIFO_DATA = 0x17,
    /*! \brief Sensor Data Availability with the Locking Mechanism*/
    STATUSINT = 0x2D,
    /*! \brief Ouput Data Over Run and Data Availability*/
    STATUS0 = 0x2E,
    /*! \brief Miscellaneous status: any motion, no motion, significan motrion, pedometer, tap*/
    STATUS1 = 0x2F,
    /*! \brief Sample time stamp lower bits*/
    TIMESTAMP_LOW = 0x30,
    /*! \brief Sample time stamp middle bits*/
    TIMESTAMP_MID = 0x31,
    /*! \brief Sample time stamp higher bits*/
    TIMESTAMP_HIGH = 0x32,
    /*! \brief Temperature Output Data*/
    TEMP_OUT_L = 0x33,
    /*! \brief Temperature Output Data*/
    TEMP_OUT_H = 0x34,
    /*! \brief Accel X Output Data lower bits*/
    ACCEL_X_L = 0x35,
    /*! \brief Accel X Output Data higher bits*/
    ACCEL_X_H = 0x36,
    /*! \brief Accel Y Output Data lower bits*/
    ACCEL_Y_L = 0x37,
    /*! \brief Accel Y Output Data higher bits*/
    ACCEL_Y_H = 0x38,
    /*! \brief Accel Z Output Data lower bits*/
    ACCEL_Z_L = 0x39,
    /*! \brief Accel Z Output Data higher bits*/
    ACCEL_Z_H = 0x3A,
    /*! \brief Gyro X Output Data lower bits*/
    GYRO_X_L = 0x3B,
    /*! \brief Gyro X Output Data higher bits*/
    GYRO_X_H = 0x3C,
    /*! \brief Gyro Y Output Data lower bits*/
    GYRO_Y_L = 0x3D,
    /*! \brief Gyro Y Output Data higher bits*/
    GYRO_Y_H = 0x3E,
    /*! \brief Gyro Z Output Data lower bits*/
    GYRO_Z_L = 0x3F,
    /*! \brief Gyro Z Output Data higher bits*/
    GYRO_Z_H = 0x40,
    /*! \brief Calibration-On-Demand status register*/
    COD_STATUS = 0x46,
    /*! \brief General Purpose register*/
    dQW_L = 0x49,
    /*! \brief General Purpose register*/
    dQW_H = 0x4A,
    /*! \brief General Purpose register*/
    dQX_L = 0x4B,
    /*! \brief RESERVED*/
    dQX_H = 0x4C,
    /*! \brief General Purpose register*/
    dQY_L = 0x4D,
    /*! \brief RESERVED 1*/
    dQY_H = 0x4E,
    /*! \brief RESERVED 2*/
    dQZ_L = 0x4F,
    /*! \brief RESERVED 3*/
    dQZ_H = 0x50,
    /*! \brief General Purpose register*/
    dVX_L = 0x51,
    /*! \brief General Purpose register*/
    dVX_H = 0x52,
    /*! \brief General Purpose register*/
    dVY_L = 0x53,
    /*! \brief General Purpose register*/
    dVY_H = 0x54,
    /*! \brief General Purpose register*/
    dVZ_L = 0x55,
    /*! \brief General Purpose register*/
    dVZ_H = 0x56,
    /*! \brief Axis, direction, number of detected tap*/
    TAP_STATUS = 0x59,
    /*! \brief low byte of step count of pedometer*/
    STEP_CNT_LOW = 0x5A,
    /*! \brief middle byte of step count of pedometer*/
    STEP_CNT_MIDL = 0x5B,
    /*! \brief high byte of step count of pedometer*/
    STEP_CNT_HIGH = 0x5C,

    /*! \brief Soft Reset Register*/
    SOFT_RESET = 0x60,
};

/**
 * @addtogroup QMI8658A_CTRL9_COMMANDS
 * @{
 *
 * Comandos para configurar el sensor IMU.
 */
enum QMI8658A_CTRL9_COMMANDS
{
    /*! \brief Set up the and enable Wake on Motion WoM*/
    CTRL_CMD_WRITE_WOM_SETTING = 0x08,
    /*! \brief Configure Tap detection*/
    CTRL_CMD_TAP = 0x0C,
    /*! \brief Configure Pedometer*/
    CTRL_CMD_PEDOMETER = 0x0D,
    /*! \brief Configure Any motion/ no motion / significant motion detection*/
    CTRL_CMD_CONFIGURE_MOTION = 0x0E,
    /*! \brief Reset pedometer count (step counter)*/
    CTRL_CMD_RESET_PEDOMETER = 0x0F,
};

/**
 * @}
 *
 */

/**
 * @addtogroup QMI8658A_CTRL9_TAP
 * @{
 *
 * Configuración del tap del sensor IMU.
 */

enum QMI8658_WakeOnMotion
{
    /*! \brief high threshold - large motion needed to wake*/
    QMI8658WoMThreshold_high = 128,
    /*! \brief Low threshold - small motion needed to wake*/
    WOM_ENABLE = 0x01,
};

/**
 * @}
 *
 */

/**
 * @addtogroup CTRL2 Accelerometer settings
 *
 * @{
 *
 */

/**
 * @brief Enumeración de las tasas de salida de datos del sensor IMU.
 *
 */
enum QMI8658_ODR
{
    /*! @brief 7174.4 Hz*/
    ODR_ACC_7174 = 0x00,
    /*! @brief 3587.2 Hz*/
    ODR_ACC_3587 = 0x01,
    /*! @brief 1793.6 Hz*/
    ODR_ACC_1793 = 0x02,
    /*! @brief 896.8 Hz*/
    ODR_ACC_896 = 0x03,
    /*! @brief 448.4 Hz*/
    ODR_ACC_448 = 0x04,
    /*! @brief 224.2 Hz*/
    ODR_ACC_224 = 0x05,
    /*! @brief 112.1 Hz*/
    ODR_ACC_112 = 0x06,
    /*! @brief 56.1 Hz*/
    ODR_ACC_56 = 0x07,
    /*! @brief 28.1 Hz*/
    ODR_ACC_28 = 0x08,
};

/**
 * @brief Enumeración de la escala de aceleración del sensor IMU.
 *
 */
enum QMI8658_aFS
{
    /*! @brief 2g*/
    aFS_2g = 0x00,
    /*! @brief 4g*/
    aFS_4g = 0x01,
    /*! @brief 8g*/
    aFS_8g = 0x02,
    /*! @brief 16g*/
    aFS_16g = 0x03,
};

/**
 * @brief Enumeración de los modos de auto test del sensor IMU.
 *
 */
enum QMI8568_aST
{
    /*! @brief Dissable self test mode*/
    DIS_self_test = 0x00,
    /*! @brief Enable self test mode*/
    EN_self_test = 0x01,
};

/**
 * @}
 *
 */


/**
 * @addtogroup CTRL3 Gyroscope settings
 *
 * @{
 *
 */

/**
 * @brief Enumeración de las tasas de salida de datos del sensor IMU.
 *
 */
enum QMI8658_ODR_GYRO
{
    /*! @brief 7174.4 Hz*/
    ODR_GYRO_7174 = 0x00,
    /*! @brief 3587.2 Hz*/
    ODR_GYRO_3587 = 0x01,
    /*! @brief 1793.6 Hz*/
    ODR_GYRO_1793 = 0x02,
    /*! @brief 896.8 Hz*/
    ODR_GYRO_896 = 0x03,
    /*! @brief 448.4 Hz*/
    ODR_GYRO_448 = 0x04,
    /*! @brief 224.2 Hz*/
    ODR_GYRO_224 = 0x05,
    /*! @brief 112.1 Hz*/
    ODR_GYRO_112 = 0x06,
    /*! @brief 56.1 Hz*/
    ODR_GYRO_56 = 0x07,
    /*! @brief 28.1 Hz*/
    ODR_GYRO_28 = 0x08,
};

/**
 * @brief Enumeración de la escala de giro del sensor IMU.
 *
 */
enum QMI8658_gFS
{
    /*! @brief 16 dps*/
    gFS_16dps = 0x00,
    /*! @brief 32 dps*/
    gFS_32dps = 0x01,
    /*! @brief 64 dps*/
    gFS_64dps = 0x02,
    /*! @brief 128 dps*/
    gFS_128dps = 0x03,
    /*! @brief 256 dps*/
    gFS_256dps = 0x04,
    /*! @brief 512 dps*/
    gFS_512dps = 0x05,
    /*! @brief 1024 dps*/
    gFS_1024dps = 0x06,
    /*! @brief 2048 dps*/
    gFS_2048dps = 0x07,

};

enum QMI8568_gST
{
    /*! @brief Disable gyro self test*/
    DIS_SELF_TEST = 0x00,
    /*! @brief Enable Self test*/
    EN_SELF_TEST = 0x01,
};

/**
* @}
}



/**
* @addtogroup IMU_FUNCS
*
* @{
*
* Funciones para configurar el sensor IMU, habilitar interrupciones y leer datos.
*
*/
void tap_config(void);
void tap_read(void);
void Non_SyncSample_mode(void);
void QMI8658_enable_interrupt(void);
void QMI8658_init(void);
/**
 * @}
 *
 */

#endif
