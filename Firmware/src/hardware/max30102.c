/**
 * @file max30102.c
 *
 * @brief Archivo con la implementación de funciones para el manejo del modulo RTC.
 *
 * Este archivo contiene la implementación de funciones para el manejo del módulo RTC, desde
 * la inicialización hasta la lectura de datos.
 *
 * @authors Maria Del Mar Arbelaez Sandoval
 *          Manuel Santiago Velasquez
 *          Julián Mauricio Sánchez Ceballos
 *
 * @see ds1302.h
 *
 * @date 21/11/2024
 *
 * @version 0.1
 *
 *
 **/
#include "../../include/hardware/max30102.h"
#include <string.h> 

sense_struct_t FIFO;

static inline void max_write_reg(uint8_t reg, uint8_t value){
    i2c_write_byte(I2C_PORT,MAX_ADDR,reg,value);
}
static inline uint8_t max_read_reg(uint8_t reg){
    return i2c_read_byte (I2C_PORT, MAX_ADDR, reg);
}

void max_burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length){
    i2c_read_nbytes (I2C_PORT, MAX_ADDR, baseAddress, buffer, length);
}

uint8_t pulse_getPartId(){
    return max_read_reg(PART_ID_REG);
}

void pulse_SPO2_config(){
    SPO2_config_t config;
    config.WORD = max_read_reg(SPO2_CONFIG_REG);

    config.BITS.led_pw=MAX_PW_410US_18BITS;
    config.BITS.sample_rate=MAX_SR_400HZ;
    config.BITS.adc_range=MAX_ADC_RGE_4096;
    max_write_reg(SPO2_CONFIG_REG,config.WORD);
}

void pulse_FIFO_config(){
    FIFO_config_t config;
    config.BITS.FIFO_AFULL=0; //number of bits till irq, prob not use it
    config.BITS.FIFO_ROLLOVER=1;
    config.BITS.SAMPLE_AVG=MAX_SA_8SAMPLE; //Gotta check that
    max_write_reg(FIFO_CONFIG_REG,config.WORD);
}

void pulse_resetFifo(){
    max_write_reg(FIFO_WR_PTR_REG, 0);
    max_write_reg(FIFO_RD_PTR_REG, 0);
    max_write_reg(OVERFLOW_COUNTER_REG, 0);
}

void pulse_shutdown(){
    uint8_t modeConfig = max_read_reg(MODE_CONFIG_REG);
    modeConfig |= (1<<7); //shutdown data camp
    max_write_reg(MODE_CONFIG_REG, modeConfig);
}

void pulse_resume(){
    uint8_t modeConfig = max_read_reg(MODE_CONFIG_REG);
    modeConfig &= ~(1<<7);
    max_write_reg(MODE_CONFIG_REG, modeConfig);
}

void pulse_enableSlots() {
    uint8_t config=(SLOT_RED_LED)|(SLOT_IR_LED<<4);
    max_write_reg(MLED_MODE_CTRL1_REG,config);
}


void max_init() {
    // Check that a MAX30105 is connected
    if (pulse_getPartId() != MAX_PART_ID) {
        printf("UH-oh MAX30102 not connected, cry a river :(\n");
        return;
    } else printf("MAX30102 connected :)\n");
    
    //configure FIFO :/
    pulse_FIFO_config();

    //SETTING PULSE ONLY MODE
    max_write_reg(MODE_CONFIG_REG,MAX_MODE_MULTI);

    //config SPO2 register
    pulse_SPO2_config();

    //setting LED current to the max
    max_write_reg(LED1_PULSE_AMP_REG,MAX30102_LED_CURR_7_6MA);
    max_write_reg(LED2_PULSE_AMP_REG,MAX30102_LED_CURR_7_6MA);

    pulse_enableSlots();
    //Clear FIFO yay
    pulse_resetFifo();
    if(max_read_reg(FIFO_RD_PTR_REG)==max_read_reg(FIFO_WR_PTR_REG)){
        printf("FIFO cleared, config done. \n");
    }
    else{printf("FIFO not cleared, oops\n");}

}

//Polls the sensor for new data
//Call regularly
//If new data is available, it updates the head and tail in the main struct
//Returns number of new samples obtained
uint16_t pulse_checkFIFO(void){
    //Read register FIDO_DATA in (3-byte * number of active LED) chunks
    //Until FIFO_RD_PTR = FIFO_WR_PTR

    uint8_t readPtr = max_read_reg(FIFO_RD_PTR_REG);
    uint8_t writePtr = max_read_reg(FIFO_WR_PTR_REG);
    int8_t numberOfSamples = 0;
    //printf("Rptr: %x, wptr: %x, samples: %d \n",readPtr,writePtr,writePtr-readPtr);
    //Do we have new data?
    if (readPtr != writePtr){
        //Calculate the number of readings we need to get from sensor
        numberOfSamples = writePtr - readPtr;
        if (numberOfSamples < 0) numberOfSamples += 32; //Wrap condition

        //We now have the number of readings, now calc bytes to read
        //For this example we are just doing Red and IR (3 bytes each)
        uint8_t bytesLeftToRead = numberOfSamples * 2 * 3; //two is the number of active LEDs

        while (bytesLeftToRead > 0){
            uint8_t toGet = bytesLeftToRead;
            if (toGet > I2C_BUFFER_LENGTH){
                //If toGet is 32 this is bad because we read 6 bytes (Red+IR * 3 = 6) at a time
                //32 % 6 = 2 left over. We don't want to request 32 bytes, we want to request 30.
                //32 % 9 (Red+IR+GREEN) = 5 left over. We want to request 27.

                toGet = I2C_BUFFER_LENGTH - (I2C_BUFFER_LENGTH % (2 * 3)); //Trim toGet to be a multiple of the samples we need to read
            }
            bytesLeftToRead -= toGet;
            //printf("Toread: %d, toget %d\n",bytesLeftToRead,toGet);

            while (toGet > 0){
                // read 1 byte from registers (values are discarded)
                uint8_t reg_INTR1 = max_read_reg(IRQ_STATUS1_REG); 
                uint8_t reg_INTR2 = max_read_reg(IRQ_STATUS2_REG); 
                uint8_t buffer[6];
                max_burstRead(FIFO_DATA_REG,buffer,6);

                FIFO.head++; //Advance the head of the storage struct
                FIFO.head %= 4; //Wrap condition

                //red
                FIFO.red[FIFO.head] = (buffer[0] << 16 | buffer[1] << 8 | buffer[2]) & 0x03FFFF; //Store this reading into the sense array
                //IR
		        FIFO.IR[FIFO.head] = (buffer[3] << 16 | buffer[4] << 8 | buffer[5]) & 0x03FFFF;
                
                toGet -= 2 * 3; //Bytes read
            }
        } //End while (bytesLeftToRead > 0)
    } //End readPtr != writePtr
    return (numberOfSamples); //Let the world know how much new data we found
}

//Check for new data but give up after a certain amount of time
//Returns true if new data was found
//Returns false if new data was not found
bool pulse_waitCheck(uint8_t maxTimeToCheck)
{
    uint32_t start = time_us_32();
    while(time_us_32()-start<maxTimeToCheck*1000){
        if(pulse_checkFIFO())return true;
    }
    printf("No data!");
    return false;
}

//Report the most recent IR value
uint32_t pulse_getIR(void){
  //Check the sensor for new data for 250ms
    if(pulse_waitCheck(250)){
        //printf(">IR:%d,Red:%d\r\n",FIFO.IR[FIFO.head],FIFO.red[FIFO.head]);
        return (FIFO.IR[FIFO.head]);}
    else
        return(0); //Sensor failed to find new data
}