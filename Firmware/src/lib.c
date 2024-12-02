/**
 * @file lib.c
 * 
 * @brief Archivo que contiene la implementación de las funciones del smartwatch.
 * 
 * Este archivo contiene la implementación de las funciones del smartwatch, es una libreria de
 * alto nivel que contiene las funciones de inicialización y configuración del smartwatch. utiliza el SDK
 * de pico para la configuración de los pines y logica del smartwatch.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see lib.h
 * @see Firmware.c
 * 
 * @date 25/11/2024
 */

#include "../include/lib.h"

extern volatile flags_t smartwatch_flags;



/**
 * @brief Función para configurar los clocks del sistema.
 * 
 * Esta función configura los clocks del sistema, estableciendo la frecuencia del PLL del sistema y configurando los clocks de los periféricos.
 * 
 */
static void config_clocks (void) {
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        PLL_SYS_KHZ * 1000,
        PLL_SYS_KHZ * 1000);
        
}


static void smartwatch_config_interrupts (void) {
    gpio_init(INT1_PIN);
    gpio_set_dir(INT1_PIN, GPIO_IN);

    gpio_set_irq_enabled_with_callback(INT1_PIN, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}



void gpio_callback(uint gpio, uint32_t events) {

    // clasificar la interrupcion
    uint8_t move_event = get_status1();

    if (move_event & WOM_INT) {
        smartwatch_flags.wom_flag = 1;
    }

    if (move_event & STEP_INT) {
        smartwatch_flags.step_flag = 1;
    }

}

void smartwatch_init(void)
{
    // configurar pll y clocks
    config_clocks();

    // inicializar imu y display
    LCD_init(HORIZONTAL);
    set_pwm(100);
    LCD_Clear(BLUE | GREEN);
    QMI8658_init();

    while (true)
    {
        // leer los pasos
        uint32_t steps = read_imu_step_count();
        printf("Steps: %d\n", steps);

        sleep_ms(1000);
    }


}