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

#define DISP_HOR_RES 240 // Resolución horizontal del display
#define DISP_VER_RES 240 // Resolución vertical del display

static lv_disp_draw_buf_t disp_buf;
static lv_color_t buf0[DISP_HOR_RES * DISP_VER_RES/2];
static lv_color_t buf1[DISP_HOR_RES * DISP_VER_RES/2];
static lv_disp_drv_t disp_drv;

static lv_obj_t *screen_main;
static lv_obj_t *label_time;
static lv_obj_t *label_date;
static lv_obj_t *label_battery;
static lv_obj_t *label_calories;
static lv_obj_t *label_distance;
static lv_obj_t *label_heart;
static lv_obj_t *label_steps;

static lv_obj_t *bar_steps;

static lv_obj_t *arc_calories, *arc_distance;


static lv_obj_t *heart_circle;
static lv_obj_t *label_pulse;





static struct repeating_timer lvgl_timer;

static void disp_flush_cb(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);

static void dma_handler(void);
static void timer_callback(lv_timer_t * timer);
static void create_screen1 (void);
static void create_screen2 (void);
static void update_main_screen(void);
uint32_t steps_count = 0;
uint8_t heartbeat_avg,heartbeat = 0;
char weekday_list [][3] = {"SUN","MON","TUE","WED","THU","FRI","SAT"};

datetime_t t ={
    .year  = 1523,
    .month = 06, //june
    .day   = 05,
    .dotw  = 5, // 0 is Sunday, so 5 is Friday
    .hour  = 15,
    .min   = 45,
    .sec   = 06
};


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

/**
 * @brief Función de callback para la animación del indicador de latidos.
 * 
 * @param obj Objeto de LVGL.
 * @param value Valor de la animación.
 * 
 * @return none
 */
static void heart_pulse_cb(void *obj, int32_t value) {
    lv_obj_set_style_radius(obj, value, 0);
    lv_obj_set_size(obj, value *2, value *2);
    lv_obj_align(obj, LV_ALIGN_CENTER, 50, -10);

}


/**
 * @brief Función para crear el indicador de latidos.
 * 
 * Esta función crea el indicador de latidos, un círculo que se expande y contrae para simular un latido.
 * 
 * @param parent Objeto padre de LVGL, en este caso la pantalla principal.
 * 
 * @return none
 */
static void create_heart_pulse_indicator (lv_obj_t *parent) {
    heart_circle = lv_obj_create(parent);
    lv_obj_set_size(heart_circle, 10, 10);
    lv_obj_set_style_radius(heart_circle, 5, 0);
    lv_obj_set_style_bg_color(heart_circle, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(heart_circle, LV_ALIGN_CENTER, 50, -10);


    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, heart_circle);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)heart_pulse_cb);
    lv_anim_set_values(&a, 13, 17);
    lv_anim_set_time(&a, 400);
    lv_anim_set_playback_time(&a, 400);
    lv_anim_set_playback_time(&a, 400);
    lv_anim_set_repeat_delay(&a, 200);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);

    label_pulse = lv_label_create(parent);
    lv_label_set_text(label_pulse, "72 bpm");
    lv_obj_set_style_text_font(label_pulse, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_pulse, heart_circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

}

/**
 * @brief Función para crear la pantalla principal.
 * 
 * Esta función crea la pantalla principal del smartwatch, con la hora, la fecha, la batería, los pasos, las calorías, los latidos y la distancia.
 * 
 * @return none
 */
static void create_main_screen (void) {
    
    screen_main = lv_obj_create(NULL);

    // configuración del widget de hora
    label_time = lv_label_create(screen_main);
    lv_label_set_text(label_time, "16:28");
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_48, 0);
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, -70);

    // configuración del widget de fecha (debajo de la hora)
    label_date = lv_label_create(screen_main);
    lv_label_set_text(label_date, "04/12/2024 WED");
    lv_obj_set_style_text_font(label_date, &lv_font_montserrat_18, 0);
    lv_obj_align(label_date, LV_ALIGN_CENTER, 0, -40);


    // indicador de bateria
    label_battery = lv_label_create(screen_main);
    lv_label_set_text(label_battery, LV_SYMBOL_BATTERY_FULL "100%");
    lv_obj_align(label_battery, LV_ALIGN_TOP_RIGHT, -90, 7);


    // barra progreso de los pasos 
    bar_steps = lv_bar_create(screen_main);
    lv_obj_set_size(bar_steps, 100, 20);
    lv_bar_set_range(bar_steps, 0, 8000);
    lv_bar_set_value(bar_steps, 600, LV_ANIM_OFF);
    lv_obj_align(bar_steps, LV_ALIGN_CENTER, -50, -10);

    lv_obj_t *label_steps = lv_label_create(screen_main);
    lv_label_set_text(label_steps, "600/8000 steps");
    lv_obj_set_style_text_font(label_steps, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_steps, bar_steps, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);

    // barra progreso de las calorias y distancia 
    arc_calories = lv_arc_create(screen_main);
    lv_obj_set_size(arc_calories, 45, 45);
    lv_arc_set_bg_angles(arc_calories, 0, 360);
    lv_obj_remove_style(arc_calories, NULL, LV_PART_KNOB);
    lv_arc_set_angles(arc_calories, 0, 0);
    lv_arc_set_value(arc_calories, 50);
    lv_obj_align(arc_calories, LV_ALIGN_CENTER, -40, 45);

    lv_obj_t *label_calories = lv_label_create(screen_main);
    lv_label_set_text(label_calories, "250/5000 \n     cal");
    lv_obj_set_style_text_font(label_calories, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_calories, arc_calories, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);


    // arco distancia
    arc_distance = lv_arc_create(screen_main);
    lv_obj_set_size(arc_distance, 45, 45);
    lv_arc_set_bg_angles(arc_distance, 0, 360);
    lv_obj_remove_style(arc_distance, NULL, LV_PART_KNOB);
    lv_arc_set_angles(arc_distance, 0, 0);
    lv_arc_set_value(arc_distance, 50);
    lv_obj_align(arc_distance, LV_ALIGN_CENTER, 40, 45);

    lv_obj_t *label_distance = lv_label_create(screen_main);
    lv_label_set_text(label_distance, "0.5/8 \n     Km");
    lv_obj_set_style_text_font(label_distance, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_distance, arc_distance, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);


    create_heart_pulse_indicator(screen_main);
}


/**
 * @brief Funcion que se utiliza para refrescar la pantalla
 * 
 * Refresca la pantalla transmitiendo los datos de color al display utilizando DMA.
 * 
 * @param disp Puntero a la estructura de la pantalla
 * @param area Área de la pantalla que se va a refrescar
 * @param color_p Puntero al buffer de color
 * 
 */
void disp_flush_cb(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    // Configurar la ventana de dibujo en la pantalla
    LCD_1IN28_SetWindows(area->x1, area->y1, area->x2, area->y2);
    
    // Enviar datos utilizando DMA
    dma_channel_configure(
        dma_tx,
        &c,
        (volatile void *)&spi_get_hw(SPI_PORT)->dr, // Dirección destino: Registro SPI
        color_p,                                    // Dirección fuente: Buffer de color
        ((area->x2 + 1 - area-> x1)*(area->y2 + 1 - area -> y1))*2, // Cantidad de datos
        true); // Iniciar transferencia automáticamente
}


/**
 * @brief Función que indica que el refresco de la pantalla está listo
 * 
 * Esta función se llama cuando el refresco de la pantalla está listo.
 * 
 * @param none
 * 
 * @return none
 */
static void dma_handler(void)
{
    uint32_t status = dma_channel_get_irq0_status(dma_tx);
    printf("status: %08x\n", status);
    if (status) {
        dma_channel_acknowledge_irq0(dma_tx);
        lv_disp_flush_ready(&disp_drv);         /* Indicate you are ready with the flushing*/
        printf("DMA transfer complete\n");
    } else {
        printf("no DMA interrupot triggered. \n");
    }
}


/**
 * @brief Funcion que inicializa la libreria de LVGL
 * 
 * Esta función inicializa la libreria de LVGL, crea un timer que se encarga de manejar los eventos de la interfaz grafica
 * 
 * @return none
 */
static void lvgl_init_library (void) {

    add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL, &lvgl_timer);

    // Inicialización de LVGL
    lv_init();

    lv_disp_draw_buf_init(&disp_buf, buf0, buf1, DISP_HOR_RES * DISP_VER_RES / 2); 
    lv_disp_drv_init(&disp_drv);    
    disp_drv.flush_cb = disp_flush_cb;
    disp_drv.draw_buf = &disp_buf;        
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);

    dma_channel_set_irq0_enabled(dma_tx, true);
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);
}



static bool repeating_lvgl_timer_callback (struct repeating_timer *t) {
   
    lv_tick_inc(5);
    return true;
}

void update_main_screen(void) {
    // Actualizar hora y fecha
    //DS1302_read(&t); // Leer del RTC

    // Actualizar hora y fecha con las funciones que hizo maria


    char time_str[8], date_str[16], steps_str[8], heart_str[16], distance_str[16], cal_str[16];
    snprintf(time_str, sizeof(time_str), "%02d:%02d", t.hour, t.min);
    snprintf(date_str, sizeof(date_str), "%02d/%02d/%04d %s", t.day, t.month, t.year, weekday_list[t.dotw]); // Día como texto con anyo
    lv_label_set_text(label_time, time_str);
    lv_label_set_text(label_date, date_str);

    // Actualizar barra de pasos
    lv_bar_set_value(bar_steps, steps_count, LV_ANIM_OFF); // Unir steps count con el output de la IMU

    // Actualizar valores de calorías, latidos y distancia (con datos simulados)
    //leer steps
    //leer bpm
    snprintf(cal_str, sizeof(cal_str), "%d Kcal", (uint32_t)(((float)steps_count * 0.04f) + (0.063f * (float)heartbeat_avg)));
    snprintf(distance_str, sizeof(distance_str), "%.1f Km", 0.75*steps_count/1000);
    snprintf(heart_str, sizeof(heart_str), "%d BPM", heartbeat);
    lv_label_set_text(label_calories, distance_str);
    lv_label_set_text(label_distance, cal_str);
    lv_label_set_text(label_heart, heart_str);

}

int smartwatch_init(void)
{
    
    
    // Configuración de PLL y clocks
    config_clocks();

    // Inicialización UART para debug
    stdio_init_all();
    sleep_ms(3000);

    // Inicialización del hardware del LCDm IMU y RTC
    LCD_init(HORIZONTAL);
    set_pwm(100);
    QMI8658_init();
    DS1302_init(&t);

    // Actualizacion de datos RTC, Hearbeats, Steps, Calories
    // update_main_screen();

    // Crear las pantallas
    create_main_screen();

    lv_scr_load(screen_main);


    // Bucle principal para LVGL
    while (true)
    {

        lv_task_handler(); 
        sleep_ms(5); // Ajustar al ciclo de tiempo necesario
    }

    return 0;
}