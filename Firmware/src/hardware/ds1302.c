/**
 * @file ds1302.c
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

#include "../../include/hardware/ds1302.h"

//Como se trabajan los números en BCD en el RTC se generan estas funciones
static inline uint8_t dec_to_bcd(uint8_t value){
    return ((value / 10)<<4) +(value % 10);
}

static inline uint8_t bcd_to_dec(uint8_t value){
    return ((value>>4)*10) + (value & 0xF);
}

uint8_t getReg(uint8_t regAddress)
{
    SPI0_beginTransmission(regAddress | READ_FLAG);
    uint8_t regValue = SPI0_ReadByte();
    SPI0_endTransmission();
    return regValue;
}

void setReg(uint8_t regAddress, uint8_t regValue)
{
    SPI0_beginTransmission(regAddress);
    SPI0_WriteByte(regValue);
    SPI0_endTransmission();
}

void DS1302_init(datetime_t* backup_time){
    SPI0_init();
    if (GetIsWriteProtected()){ //esto debería ser un 80
        printf("RTC was write protected, enabling writing now\n");
        SetIsWriteProtected(false);
    }
    if (!GetIsRunning()){
        printf("RTC was not actively running, starting now\n");
        SetDateTime(backup_time);
        SetIsRunning(true);
    }

    if (!IsDateTimeValid()) {
        printf("RTC lost confidence in the DateTime!\n");
        SetDateTime(backup_time);
    }
}

bool GetIsWriteProtected(){
    uint8_t wp = getReg(WRITE_PROTECT_REG);

    return !!(wp & 1<<7); //la doble negación lo vuelve un booleano, interesting
}

void SetIsWriteProtected(bool WriteProtect){
    uint8_t wp = getReg(WRITE_PROTECT_REG);
    if (WriteProtect){
    wp |= 1<<7;
    } else {
        wp &= ~(1<<7);
    }
    setReg(WRITE_PROTECT_REG, wp);
}

//activo en bajo (RECORDAR)
bool GetIsRunning(){
    uint8_t ch = getReg(SECONDS_REG);
    return !(ch>>7);
}

//ACTIVO EN BAJO
void SetIsRunning(bool run){
    uint8_t ch = getReg(SECONDS_REG);
    if (run){
        ch &= ~(1<<7);
    } else{
        ch |= (1<<7);
    }
    setReg(SECONDS_REG, ch);
}

void SetDateTime(datetime_t* dt){
    printf("Setting Time!!");

    SPI0_beginTransmission(BURST_MODE_REG);
    SPI0_WriteByte(dec_to_bcd(dt->sec % 60));
    SPI0_WriteByte(dec_to_bcd(dt->min % 60));
    SPI0_WriteByte(dec_to_bcd(dt->hour % 24)); // 24 hour mode only
    SPI0_WriteByte(dec_to_bcd(dt->day % 31));
    SPI0_WriteByte(dec_to_bcd(dt->month % 12));
    SPI0_WriteByte(dec_to_bcd(dt->dotw) % 7);
    SPI0_WriteByte(dec_to_bcd(dt->year % 100));
    SPI0_WriteByte(0); // no write protect, as all of this is ignored if it is protected

    SPI0_endTransmission();
    //write the year upper three digits so that we dont lose that and can add weird year dates
    
    SetMemory(0, dt->year/100);

}

void GetDateTime(datetime_t* dt){
    SPI0_beginTransmission(BURST_MODE_REG | READ_FLAG);

    dt->sec = bcd_to_dec( SPI0_ReadByte() & 0x7F); //para que no entre el clock halt
    dt->min = bcd_to_dec( SPI0_ReadByte());
    dt->hour = bcd_to_dec( SPI0_ReadByte());
    dt->day = bcd_to_dec( SPI0_ReadByte());
    dt->month = bcd_to_dec( SPI0_ReadByte());
    dt->dotw = bcd_to_dec( SPI0_ReadByte());
    dt->year = bcd_to_dec( SPI0_ReadByte()); //this is just the decades 

    SPI0_ReadByte();  // throwing away write protect flag
    SPI0_endTransmission();

    dt->year=dt->year+GetMemory(0)*100;

}

void SetMemory(uint8_t memoryAddress, uint8_t value){
    // memory addresses interleaved read and write addresses
    // so we need to calculate the offset
    uint8_t address = memoryAddress * 2 + RAM_START_ADDR;
    if (address <= RAM_END_ADDR) setReg(address, value);
    else printf("Memory address out of range!!!!\n");
}

uint8_t GetMemory(uint8_t memoryAddress){
    uint8_t value = 0;
    // memory addresses interleaved read and write addresses
    // so we need to calculate the offset
    uint8_t address = memoryAddress * 2 + RAM_START_ADDR;
    if (address <= RAM_END_ADDR) value = getReg(address);
    else printf("Memory address out of range :((((");    
    return value;
}

bool IsDateTimeValid(){
    datetime_t dt;
    GetDateTime(&dt);
    return DateIsValid(&dt);
}

bool DateIsValid(datetime_t* dt){
    // this just tests the most basic validity of the value ranges
    // and valid leap years
    // It does not check any time zone or daylight savings time
    //this is if the format is "ok"
    if ((dt->month < 13) &&
        (dt->day < 32) &&
        (dt->hour < 24) &&
        (dt->min < 60) &&
        (dt->sec < 60))
    {
    // days in a month tests
    if (dt->month == 2){
        if (dt->day > 29) return false;
        else if (dt->day > 28) {
            // leap day
            // check year to make sure its a leap year
            if ((dt->year % 4) != 0) return false;
            if ((dt->year % 100) == 0 && (dt->year % 400) != 0)  return false;
        }
    }
    else if (dt->day == 31){
        if ((((dt->month - 1) % 7) % 2) == 1) return false;
    }
    
    return true;
    }
    return false;
}

void print_datetime(datetime_t *dt){
    printf("Printing datetime:");
    char datetime_buf[256];
    char *datetime_str = &datetime_buf[0];
    datetime_to_str(datetime_str, sizeof(datetime_buf), dt);
    printf("\r%s      \n", datetime_str);

}