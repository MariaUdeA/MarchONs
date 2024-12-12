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

static lv_indev_drv_t indev_en;
static lv_group_t *group;
static lv_obj_t *screen1;
static lv_obj_t *label_steps;
static lv_obj_t *label_time;
static lv_obj_t *label_date;
static lv_obj_t *label_battery;
static lv_obj_t *label_calories;
static lv_obj_t *label_distance;
static lv_obj_t *label_heart;

static lv_obj_t *bar_steps;


static lv_obj_t *arc_calories, *arc_distance;

static lv_obj_t *heart_circle;
static lv_obj_t *label_pulse;


static struct repeating_timer lvgl_timer;
static struct repeating_timer ms_timer;

static void disp_flush_cb(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);

static void dma_handler(void);
static void timer_callback(lv_timer_t * timer);
static void create_screen1 (void);
static void create_screen2 (void);

datetime_t t ={
    .year  = 2025,
    .month = 1, //june
    .day   = 1,
    .dotw  = 3, // 0 is Sunday, so 5 is Friday
    .hour  = 23,
    .min   = 59,
    .sec   = 00
};

timer_flags_t flags;


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

static void dma_handler(void)
{
    uint32_t status = dma_channel_get_irq0_status(dma_tx);
    //printf("status: %08x\n", status);
    if (status) {
        dma_channel_acknowledge_irq0(dma_tx);
        lv_disp_flush_ready(&disp_drv);         /* Indicate you are ready with the flushing*/
        //printf("DMA transfer complete\n");
    } else {
        //printf("no DMA interrupt triggered. \n");
    }
}

static void heart_pulse_cb(void *obj, int32_t value) {
    lv_obj_set_style_radius(obj, value, 0);
    lv_obj_set_size(obj, value *2, value *2);
    lv_obj_align(obj, LV_ALIGN_CENTER, 50, -10);
}

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
    lv_label_set_text(label_pulse, "70 bpm");
    lv_obj_set_style_text_font(label_pulse, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_pulse, heart_circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
}

static void create_screen1 (void) {
    
    screen1 = lv_obj_create(NULL);

    // WIDGET DE LA HORA
    label_time = lv_label_create(screen1);
    lv_label_set_text(label_time, "15:45");
    lv_obj_set_style_text_font(label_time, &lv_font_montserrat_48, 0);
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, -70);

    // WIDGET DE LA FECHA (DEBAJO DE LA HORA)
    label_date = lv_label_create(screen1);
    lv_label_set_text(label_date, "10/12/2024 WED");
    lv_obj_set_style_text_font(label_date, &lv_font_montserrat_18, 0);
    lv_obj_align(label_date, LV_ALIGN_CENTER, 0, -40);


    // WIDGET DE LA BATERIA
    label_battery = lv_label_create(screen1);
    lv_label_set_text(label_battery, LV_SYMBOL_BATTERY_FULL "100%");
    lv_obj_align(label_battery, LV_ALIGN_TOP_RIGHT, -90, 7);

    // BARRA DE PROGRESO DE LOS PASOS
    bar_steps = lv_bar_create(screen1);
    lv_obj_set_size(bar_steps, 100, 20);
    lv_bar_set_range(bar_steps, 0, 1000);
    lv_bar_set_value(bar_steps, 0, LV_ANIM_OFF);
    lv_obj_align(bar_steps, LV_ALIGN_CENTER, -50, -10);

    label_steps = lv_label_create(screen1);
    lv_label_set_text(label_steps, "Steps: 0");
    lv_obj_set_style_text_font(label_steps, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_steps, bar_steps, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);


    // WIDGET DE LAS CALORIAS
    arc_calories = lv_arc_create(screen1);
    lv_obj_set_size(arc_calories, 45, 45);
    lv_arc_set_rotation(arc_calories, 270);
    lv_arc_set_bg_angles(arc_calories, 0, 360);
    lv_obj_remove_style(arc_calories, NULL, LV_PART_KNOB);
    lv_arc_set_value(arc_calories, 0);
    lv_obj_align(arc_calories, LV_ALIGN_CENTER, -40, 45);

    label_calories = lv_label_create(screen1);
    lv_label_set_text(label_calories, "0");
    lv_obj_set_style_text_font(label_calories, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_calories, arc_calories, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);


    // WIDGET DE LA DISTANCIA
    arc_distance = lv_arc_create(screen1);
    lv_obj_set_size(arc_distance, 45, 45);
    lv_arc_set_rotation(arc_distance, 270);
    lv_arc_set_bg_angles(arc_distance, 0, 360);
    lv_obj_remove_style(arc_distance, NULL, LV_PART_KNOB);
    lv_arc_set_value(arc_distance, 0);
    lv_obj_align(arc_distance, LV_ALIGN_CENTER, 40, 45);

    label_distance = lv_label_create(screen1);
    lv_label_set_text(label_distance, "0m");
    lv_obj_set_style_text_font(label_distance, &lv_font_montserrat_10, 0);
    lv_obj_align_to(label_distance, arc_distance, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);

    //Heart Pulse Indicator
    create_heart_pulse_indicator(screen1);


}

static bool repeating_lvgl_timer_callback (struct repeating_timer *t) {
    lv_tick_inc(5);
    flags.five_mil=true;
    return true;
}

static bool repeating_500ms_timer_callback(struct repeating_timer *t){
    static uint8_t count;
    static bool halfy;
    count=(count+1)%3;
    halfy=~halfy;

    flags.half=1;
    if (halfy) flags.full=1;
    if (count==2) flags.one_half=1;

    return true;
}

void smartwatch_init(){
    // Configuración de PLL y clocks
    config_clocks();

    // Inicialización USB para debug
    stdio_init_all();
    sleep_ms(3000);

    // Inicialización del hardware
    LCD_init(HORIZONTAL);
    set_pwm(100);
    QMI8658_init();
    max_init();

    //inicilizar el reloj
    DS1302_init(&t,USB_CONFIG);


    add_repeating_timer_ms(5, repeating_lvgl_timer_callback, NULL, &lvgl_timer);

    //let's see if this breaks everything! yay!
    add_repeating_timer_ms(500, repeating_500ms_timer_callback, NULL, &ms_timer);

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

    // Crear las pantallas
    create_screen1();

    // Inicializar la pantalla
    lv_scr_load(screen1);

    printf("DMA status: %08x\n", dma_channel_get_irq0_status(dma_tx));

    //adc reading for battery
    adc_init();
    adc_gpio_init(29);
    adc_select_input(3);
}

void check_for_new_day(datetime_t*now){
    datetime_t compare;
    GetDateTime(&compare);
    if (now->day != compare.day){
        printf("%d vs %d",now->day,compare.day);
        reset_imu();
        QMI8658_init();
        SetMemory(4,0);
        SetMemory(5,0);
        SetMemory(6,0);
    }
}

void save_steps(uint32_t offset, uint32_t*steps){
    *steps=offset+*steps;
    
    uint8_t step_low = *steps & 0xFF;
    uint8_t step_mid = (*steps>>8) & 0xFF;
    uint8_t step_high = (*steps>>16) &0XFF; 

    //escritura en memoria
    SetMemory(4,step_low);
    SetMemory(5,step_mid);
    SetMemory(6,step_high);  
}

void update_steps(uint32_t*steps,uint32_t offset){
    *steps = read_imu_step_count();
    save_steps(offset,steps);
    char steps_str[32];
    snprintf(steps_str, 64, "Steps: %d", (*steps%1001)); //texto de abajo
    //printf("udapted text: %s\n", steps_str);

    lv_bar_set_value(bar_steps, *steps, LV_ANIM_OFF);

    lv_label_set_text(label_steps, steps_str);
    lv_obj_align_to(label_steps, bar_steps, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);

}

void update_time(datetime_t*now){
    const char* dotw_lookup[] = {"SUN","MON","TUE","WED", "THU", "FRI","SUN"};
    GetDateTime(now);

    //hour and minutes section
    char time_str[32];
    snprintf(time_str, 64, "%.2d:%.2d",now->hour,now->min); //texto de abajo

    lv_label_set_text(label_time, time_str);
    lv_obj_align(label_time, LV_ALIGN_CENTER, 0, -70);

    //date section
    snprintf(time_str, 64,"%.2d/%.2d/%.4d %.3s",now->day,now->month,now->year,dotw_lookup[now->dotw]); //texto de abajo

    // WIDGET DE LA FECHA (DEBAJO DE LA HORA)
    lv_label_set_text(label_date, time_str);
    lv_obj_align(label_date, LV_ALIGN_CENTER, 0, -40);
}

void update_distance(uint32_t steps){
    uint8_t height=GetMemory(2);
    uint32_t distance=steps*height*0.414/100; //size of step according to height in m
    uint8_t angle=(100*distance/750)%101; //750m is the top distance to show
    
    lv_arc_set_value(arc_distance, angle);
    lv_obj_align(arc_distance, LV_ALIGN_CENTER, 40, 45);

    //hour and minutes section
    char dist_str[32];
    snprintf(dist_str, 64, "%dm",distance); //texto de abajo

    lv_label_set_text(label_distance,dist_str);
    lv_obj_align_to(label_distance, arc_distance, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);
}

void update_battery(){
    char symbol[12];

    uint16_t voltage = 33*adc_read() / (1 << 12) * 2;
    uint16_t percent=100*(voltage - 30) / (40 - 30);
    //printf("v:%d,p:%d\n",voltage,percent);
    
    if(voltage>=40){
        percent=100;
        strcpy(symbol, LV_SYMBOL_BATTERY_FULL);
    }else if(voltage>=37){
        strcpy(symbol, LV_SYMBOL_BATTERY_3);
    }else if(voltage>=35){
        strcpy(symbol, LV_SYMBOL_BATTERY_2);
    }else if(voltage>=30){
        strcpy(symbol, LV_SYMBOL_BATTERY_1);
    }else if(voltage>=25){
        strcpy(symbol, LV_SYMBOL_BATTERY_EMPTY);
    }

    char per_str[10];
    snprintf(per_str, 16, "%d%%",percent); //texto de abajo
    strcat(symbol, per_str);

    lv_label_set_text(label_battery, symbol);
    lv_obj_align(label_battery, LV_ALIGN_TOP_RIGHT, -90, 7);

}

void update_hr(uint8_t bpm){
    char bpm_str[3];
    snprintf(bpm_str, 64, "%d",bpm); //texto de abajo

    lv_label_set_text(label_pulse, bpm_str);
    lv_obj_align_to(label_pulse, heart_circle, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
}

void update_calories(uint32_t steps,uint8_t bpm){
    uint8_t weight=GetMemory(1);
    uint8_t height=GetMemory(2);
    uint8_t age=GetMemory(3);
    uint32_t dist=steps*height*0.414/100; //size of step according to height in m
    uint32_t cals=(dist/0.13)*(0.6309*bpm+0.1988*weight+0.2017*age-55.0969)/4.184;
    uint8_t cals_per=cals*100/150000; //let's say that the max is 250 kcals

    char cals_str[3];
    snprintf(cals_str, 64, "%dcals",cals); //texto de abajo

    lv_arc_set_value(arc_calories, cals_per);
    lv_obj_align(arc_calories, LV_ALIGN_CENTER, -40, 45);

    lv_label_set_text(label_calories, cals_str);
    lv_obj_align_to(label_calories, arc_calories, LV_ALIGN_OUT_BOTTOM_MID, 0, 3);

}

void end_screen(){
    lv_obj_invalidate(screen1);
    lv_task_handler(); //esto tiene que suceder cada 5ms
}

int smartwatch_main(void){
    smartwatch_init();
    flags.half=0;
    flags.full=0;
    flags.one_half=0;
    uint32_t offset=read_steps_rtc();
    printf("Actual offset:%d\n",offset);
    //inicialización de las variables de lectura de datos
    uint32_t steps = 0;
    uint8_t bpm=70;
    datetime_t now;
    GetDateTime(&now); //para evitar un bucle infinito
    printf("Set day: %d\n",now.day);

    // Bucle principal para LVGL
    while (true)
    {
        if(flags.five_mil | flags.full | flags.half | flags.one_half | pulse_getIR_flag()){
            if(pulse_getIR_flag()){

                add_sample(pulse_getIR());
                pulse_setIR_flag(false);
            }
            if(flags.half){
                update_steps(&steps,offset);
                update_battery();
                update_distance(steps);
                update_calories(steps,bpm);
                end_screen();
                flags.half=0;
            }
            if(flags.full){

                update_time(&now);
                end_screen();
                flags.full=0;
            }
            if(flags.one_half){
                uint8_t bpm_read = calculate_heart_rate();
                if(bpm_read!=255)bpm=bpm_read;
                update_hr(bpm);
                end_screen();
                flags.one_half=0;
            }
            if (flags.five_mil){  
                check_for_new_day(&now);      
                lv_obj_invalidate(screen1);
                lv_task_handler(); //esto tiene que suceder cada 5ms
                flags.five_mil=false;
            }
        }
        else{__wfi();}
    }

    return 0;
}
