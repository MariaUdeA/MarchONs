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

    /**
     * @addtogroup IMU_REGISTERS
     * @{
     * 
     * Direcciones de los registros a usar del sensor IMU.
     */
    #define IMU_ADDR    0x6B        /**< Dirección del sensor IMU (Ver el datashee del QMI8658A pg. 9)*/
    #define WHO_AM_I    0x00        /**< Dirección del registro WHO_AM_I del sensor IMU. */
    #define CAL4_L      0x11        /**< Dirección del registro CAL4_L del sensor IMU. */
    #define CAL4_H      0x12        /**< Dirección del registro CAL4_H del sensor IMU. */
    
    #define CTRL1       0x02        /**< Dirección del registro CTRL1 del sensor IMU. */
    #define CTRL7       0x08        /**< Dirección del registro CTRL7 del sensor IMU. */
    #define CTRL8       0x09        /**< Dirección del registro CTRL8 del sensor IMU. */ 
    #define CTRL9       0x0A        /**< Dirección del registro CTRL9 del sensor IMU. */

    #define STEP_CNT_L  0x5A        /**< Dirección del registro STEP_CNT_L del sensor IMU. */
    #define STEP_CNT_M  0x5B        /**< Dirección del registro STEP_CNT_M del sensor IMU. */
    #define STEP_CNT_H  0x5C        /**< Dirección del registro STEP_CNT_H del sensor IMU. */

    /**
     * @}
     * 
    */

    /**
     * @addtogroup CTRL_CMD
     * @{
     * 
     * Comandos para configurar el sensor IMU.
     */
    #define bCTRL_CMD_CONFIGURE_PEDOMETER    0x0D    /**< Comando para configurar el pedometer del sensor IMU. */
    #define bCTRL_ACCEL_EN                   0x00    /**< Habilitar el acelerómetro del sensor IMU. */
    #define bCTRL_PEDOMETER_EN               0x04    /**< Habilitar el pedometer del sensor IMU. */

    /**
     * @}
     * 
    */


   /**
    * @addtogroup CONFIG_IMU
    * 
    * @{
    * 
    * Configuración del sensor IMU.
    */
 
    /**
     * @brief Función que lee el registro WHO_AM_I del sensor IMU.
     * 
     * Esta función lee el registro WHO_AM_I del sensor IMU.
     * 
     * @return int Valor del registro WHO_AM_I.
     */
    int imu_who_am_i (void);


    /**
     * @brief Función que configura el pedometer del sensor IMU.
     * 
     * Esta función configura el pedometer del sensor IMU.
     * 
     * @return void
     */
    void configure_pedometer (void);


    /**
     * @brief Función que habilita el pedometer del sensor IMU.
     * 
     * Esta función habilita el pedometer del sensor IMU.
     * 
     * @return void
     */
    void enable_pedometer (void);

    /**
     * @brief Función que configura la interrupción del pedometer del sensor IMU.
     * 
     * Esta función configura la interrupción del pedometer del sensor IMU.
     * 
     * @return void
     * 
     * @see configure_pedometer
     */
    void configure_pedometer_interrupt (void);

    /**
     * @brief Función que inicializa el sensor IMU.
     * 
     * Esta función inicializa el sensor IMU, configurando los registros necesarios para su correcto funcionamiento.
     * 
     * @return void
     */
    void imu_init_setup (void);

    /**
     * @}
    */


    /**
     * @addtogroup READ_IMU
     * 
     * @{
     * 
     * Lectura de datos del sensor IMU.
     */
    

    uint32_t imu_read_steps (void);
    /**
     * @}
     * 
    */




#endif

