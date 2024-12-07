/**
 * @file pulse_read.h
 * 
 * @brief Archivo con la definición de funciones para el manejo del medidor de pulso.
 * 
 * Este archivo contiene la definición de funciones para el manejo del medidor de pulso, la configuración
 * del pin de interrupcion y la medición del pulso.
 * 
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 * 
 * @see pulse_read.c
 * @see max30102.h
 * @see max30102.c
 * 
 * @date 04/12/2024
 * 
 * @version 1.0
 */

#include "../include/pulse_read.h"
#define MAX_WINDOW 255


typedef struct beat_detector
{
    uint16_t sample_rate; //100
    uint8_t window_size; //10
    uint8_t smoothing_window; //5
    uint32_t sample[MAX_WINDOW];
    uint32_t timestamps[MAX_WINDOW];
    uint32_t filtered_samples[MAX_WINDOW+1];
    uint8_t round;
    uint8_t peak_len;
} beat_detector_t;

beat_detector_t detect={
    .sample_rate=50,
    .window_size=150,
    .smoothing_window=15,
    .round=0,
    .peak_len=0
};


void add_sample(uint32_t sample){
    uint32_t timestamp = time_us_32()/1000; //write it down in ms!
    detect.sample[detect.round]=sample;
    detect.timestamps[detect.round]=timestamp;
    //printf("round%d\n",detect.round);
    //printf(">sample:%d,",detect.sample[detect.round]);
    //Smooth the signal?
    uint32_t smoothed_sample;
    if (detect.round >= detect.smoothing_window-1){
        smoothed_sample=0;
        for (uint8_t i=0;i<detect.smoothing_window;i++){
            smoothed_sample+=detect.sample[detect.round-i];
        }
        smoothed_sample = smoothed_sample/detect.smoothing_window;
        //printf("Smooth:%d\r\n",smoothed_sample);
        detect.filtered_samples[detect.round]=smoothed_sample;
    }else{
        detect.filtered_samples[detect.round]=sample;
    }


    if (detect.round>=detect.window_size-1){
        for (int k = 0; k < detect.window_size-1; k++) detect.sample[k]=detect.sample[k+1];
        for (int k = 0; k < detect.window_size-1; k++) detect.timestamps[k]=detect.timestamps[k+1];
        for (int k = 0; k < detect.window_size-1; k++) detect.filtered_samples[k]=detect.filtered_samples[k+1];
    }else{
    //Maintain the size of samples and timestamps
    detect.round++;}
}

void find_peaks(uint32_t peaks[32][2]){
    //Find peaks in the filtered samples.
    detect.peak_len=0;
    if (detect.round<3){
        printf("Not enough!!!");
        return; //return peaks
    }

    //Calculate dynamic threshold based on the min and max of the recent window of filtered samples
    uint32_t min_val = ~0;
    for (uint8_t i=0;i<detect.window_size;i++){
        if(detect.filtered_samples[detect.round-i]<min_val) min_val=detect.filtered_samples[detect.round-i];
    }

    uint32_t max_val = 0;
    for (uint8_t i=0;i<detect.window_size;i++){
        if(detect.filtered_samples[detect.round-i]>max_val) max_val=detect.filtered_samples[detect.round-i];
    }

    uint32_t threshold= min_val + (max_val - min_val) * 0.5;// 50% between min and max as a threshold
    if(threshold>10000){ //pulse valid
        for (uint8_t i=1; i<detect.round-1; i++){
            if (detect.filtered_samples[i] > threshold && detect.filtered_samples[i - 1] < detect.filtered_samples[i] && detect.filtered_samples[i] > detect.filtered_samples[i + 1]){
                    peaks[detect.peak_len][0]=(uint32_t)detect.timestamps[i];
                    uint32_t placeholder=detect.filtered_samples[i];
                    peaks[detect.peak_len][1]= placeholder;
                    detect.peak_len++;
            }
        }
    }

}

uint8_t calculate_heart_rate(){
    uint32_t peaks[32][2];
    find_peaks(peaks);

    if (detect.peak_len<2){
        return 0xFF;
    }

    uint32_t intervals[32];

    for(uint8_t j = 1; j<detect.peak_len; j++){
        intervals[j-1]=peaks[j][0]-peaks[j-1][0]; //this is in ms
    }
    //la longitud final de intervals es de peak_len-1
    uint32_t average_interval=0;
    for (uint8_t i=0;i<detect.peak_len-1;i++){
        //printf("Int: %d ",intervals[i]);
        average_interval+=intervals[i];
    }
    average_interval=average_interval/(detect.peak_len-1);
    //printf("AVG: %d\n ",average_interval);

    // Convert intervals to heart rate in beats per minute (BPM)
    uint16_t heart_rate =(60000 / average_interval);  // 60 seconds per minute * 1000 ms per second
    return (uint8_t)heart_rate;

}




/*
//  Heart Rate Monitor functions takes a sample value and the sample number
//  Returns true if a beat is detected
//  A running average of four samples is recommended for display on the screen.
void get_bpm(){
    uint8_t rates[AVG_SIZE]; //Array of heart rates
    uint8_t rateSpot = 0;
    uint32_t lastBeat = 0; //Time at which the last beat occurred

    static uint16_t beatsPerMinute;
    static uint8_t beatAvg=0;

    uint32_t irValue = pulse_getIR();
    //printf(">ir:%d,",irValue);
    if (checkForBeat(irValue) == true){
        //We sensed a beat!
        uint32_t delta = (time_us_32() - lastBeat)/1000; //in ms!
        lastBeat = time_us_32();
        beatsPerMinute = 60 * 1000 / delta;
        //printf("Delta in ms: %d,bpm:%d\n",delta,beatsPerMinute);

        if (beatsPerMinute < 255 && beatsPerMinute > 20){
            rates[rateSpot++] = (uint8_t)beatsPerMinute; //Store this reading in the array
            rateSpot %= AVG_SIZE; //Wrap variable

            //Take average of readings
            beatAvg = 0;
            for (uint8_t x = 0 ; x < AVG_SIZE ; x++)
                beatAvg += rates[x];
            beatAvg /= AVG_SIZE;
            printf("IR=%d ",irValue);
            printf("BPM=%d ",beatsPerMinute);
            printf("AVG_BPM=%d\n",beatAvg);
        }
    }

    if (irValue < 50000){
        printf(" No finger?\n");
    }
}

bool checkForBeat(int32_t sample){

    //Creating local variables
    bool beatDetected = false;

    int16_t IR_AC_Signal_Previous;
    static int16_t IR_AC_Signal_Current;
    static int32_t ir_avg_reg;
    static uint8_t positiveEdge;
    static uint8_t negativeEdge;
    static int16_t IR_AC_Signal_min;
    static int16_t IR_AC_Signal_max;

    //  Save current state
    IR_AC_Signal_Previous=IR_AC_Signal_Current;
    //This is good to view for debugging
    //printf(">Signal:%d,",IR_AC_Signal_Current);

    //  Process next data sample
    int16_t IR_Average_Estimated = averageDCEstimator(&ir_avg_reg, sample);
    //printf("avg:%d\r\n",IR_Average_Estimated);

    IR_AC_Signal_Current = lowPassFIRFilter(sample - IR_Average_Estimated);
    //printf("Ac_sig%d\n",IR_AC_Signal_Current);
    //  Detect positive zero crossing (rising edge)
    if ((IR_AC_Signal_Previous < 0) & (IR_AC_Signal_Current >= 0)){
        printf("Rising!!!! ");
        IR_AC_Max = IR_AC_Signal_max; //Adjust our AC max and min
        IR_AC_Min = IR_AC_Signal_min;
        printf("Max: %d Min: %d \n",IR_AC_Max,IR_AC_Min);
        positiveEdge = 1;
        negativeEdge = 0;
        IR_AC_Signal_max = 0;

        //if ((IR_AC_Max - IR_AC_Min) > 100 & (IR_AC_Max - IR_AC_Min) < 1000)
        if ((IR_AC_Max - IR_AC_Min) > 20 & (IR_AC_Max - IR_AC_Min) < 1000){
            //Heart beat!!!
            printf("Beat!!!");
            beatDetected = true;
        }
    }
    
    //  Detect negative zero crossing (falling edge)
    if ((IR_AC_Signal_Previous > 0) & (IR_AC_Signal_Current <= 0)){
        positiveEdge = 0;
        negativeEdge = 1;
        IR_AC_Signal_min = 0;
    }

    //  Find Maximum value in positive cycle
    if (positiveEdge & (IR_AC_Signal_Current > IR_AC_Signal_Previous)){
        IR_AC_Signal_max = IR_AC_Signal_Current;
    }

    //  Find Minimum value in negative cycle
    if (negativeEdge & (IR_AC_Signal_Current < IR_AC_Signal_Previous)){
        IR_AC_Signal_min = IR_AC_Signal_Current;
    }
    return(beatDetected);
}

//  Average DC Estimator
int16_t averageDCEstimator(int32_t *p, uint16_t x)
{
  *p += ((((uint32_t) x << 15) - *p) >> 4);
  return (*p >> 15);
}

//  Low Pass FIR Filter, pray for me
int16_t lowPassFIRFilter(int16_t din){
    static int16_t cbuf[32];
    static uint8_t offset; 
    cbuf[offset] = din;
    int32_t z = mul16(FIRCoeffs[11], cbuf[(offset - 11) & 0x1F]);
  
    for (uint8_t i = 0 ; i < 11 ; i++){
        z += mul16(FIRCoeffs[i], cbuf[(offset - i) & 0x1F] + cbuf[(offset - 22 + i) & 0x1F]);
    }
    offset++;
    offset %= 32; //Wrap condition
    return(z >> 15);
}

//  Integer multiplier
int32_t mul16(int16_t x, int16_t y){
  return((uint32_t)x * (uint32_t)y);
}*/