 #include "intrinsics.h"
#include <ds18b20.h>
 
 // GPIO Konfiguration
void init_pins() {
    P1DIR |= POWER_PIN;     // P1.5 als Ausgang (Power)
    P1OUT &= ~POWER_PIN;    // Power aus
    P1DIR |= DQ_PIN;        // DQ als Ausgang
    P1OUT |= DQ_PIN;        // Setze DQ auf High (Pull-Up aktiv)
}

// DS18B20: Reset Impuls
/*
unsigned char ds18b20_reset() {
    unsigned char presence;
    P1OUT &= ~DQ_PIN;       // DQ auf Low ziehen
    P1DIR |= DQ_PIN;        // DQ als Ausgang
    DELAY_US(480);          // Halte Low für 480µs
    P1DIR &= ~DQ_PIN;       // DQ als Eingang
    DELAY_US(70);           // Warte 70µs
    presence = (P1IN & DQ_PIN); // Lese Präsenzsignal
    DELAY_US(410);          // Warte den Rest der Periode
    return presence == 0 ? 1 : 0; // 1 = Sensor vorhanden, 0 = kein Sensor
}*/
uint8_t ds18b20_reset(void)
{
    DS18B20_LO
    DELAY_US(500);                  // 480us minimum
    DS18B20_RLS
    DELAY_US(60);                   // slave waits 15-60us  // try 80 or 40
    if (DS18B20_IN) return 1;       // line should be pulled down by slave
    DELAY_US(240);                  // slave TX presence pulse 60-240us
    if (!DS18B20_IN) return 2;      // line should be "released" by slave
    return 0;
}
 
 uint16_t ds18b20_get_temp(void){
    volatile uint16_t temp = 0;
    ds18b20_reset();
    ds18b20_write_byte(DS18B20_SKIP_ROM);           // skip ROM command
    ds18b20_write_byte(DS18B20_CONVERT_T);          // convert T command
    DS18B20_HI
//    DELAY_MS(800);                                  // at least 750 ms for the default 12-bit resolution
    __delay_cycles(800000);
    ds18b20_reset();
    ds18b20_write_byte(DS18B20_SKIP_ROM);           // skip ROM command
    ds18b20_write_byte(DS18B20_READ_SCRATCHPAD);    // read scratchpad command
    temp = ds18b20_read_temp_registers();
    return temp;
 }

 void ds18b20_write_bit(uint8_t bit)
{
    DELAY_US(1);                    // recovery, min 1us
    DS18B20_HI
    if (bit) {
        DS18B20_LO
        DELAY_US(5);                // max 15us
        DS18B20_RLS                 // input
        DELAY_US(50);
    }
    else {
        DS18B20_LO
        DELAY_US(60);               // min 60us
        DS18B20_RLS                 // input
        DELAY_US(1);
    }
}

//**********************************************************************************************************************************************************
uint8_t ds18b20_read_bit()
{
    uint8_t bit=0;
    DELAY_US(1);
    DS18B20_LO
    DELAY_US(1);                    // hold min 1us
    DS18B20_RLS
    DELAY_US(5);                    // 15us window
    if (DS18B20_IN) bit = 1;
    DELAY_US(45);                   // rest of the read slot
    return bit;
}

//**********************************************************************************************************************************************************
void ds18b20_write_byte(uint8_t byte)
{
    uint8_t i;
    for(i = 8; i > 0; i--)
    {
        ds18b20_write_bit(byte & 1);
        byte >>= 1;
    }
}

//**********************************************************************************************************************************************************
uint8_t ds18b20_read_byte()
{
    uint8_t i;
    uint8_t byte = 0;
    for(i = 8; i > 0; i--)
    {
        byte >>= 1;
        if (ds18b20_read_bit()) byte |= 0x80;
    }
    return byte;
}

//**********************************************************************************************************************************************************
uint16_t ds18b20_read_temp_registers ( void )
{
    uint8_t i;
    uint16_t byte = 0;
    for(i = 16; i > 0; i--){
        byte >>= 1;
        if (ds18b20_read_bit()) {
            byte |= 0x8000;
        }
    }
    return byte;
}

//**********************************************************************************************************************************************************
uint64_t readRom (void){
    uint8_t i;
    uint64_t byte = 0;

    ds18b20_reset();
    ds18b20_write_byte(DS1820_READ_ROM);

    for(i = 64; i > 0; i--){
        byte >>= 1;
        if (ds18b20_read_bit()) {
            byte |= 0x8000000000000000;
        }
    }
    return byte;
}

//**********************************************************************************************************************************************************
/*
char* show_temp(float temp)
{
    uint16_t aux;
    char message[50];              // Puffer für die formatierte Nachricht
    aux=temp/10;
//    showChar(aux+48,pos1);
    sprintf(message, "%d", aux+48);

    aux=(int16_t)temp%10;
    sprintf(message + strlen(message), "%d", aux+48);

//    showChar(aux+48,pos2);
//    LCDMEM[pos2+1] |= 0x01;

    volatile float mantisa = temp - (int16_t)temp;
    volatile uint16_t dosDecimales = mantisa * 100;
    aux=((int)dosDecimales)/10;
    sprintf(message+ strlen(message), "%d", aux+48);
 //   showChar(aux+48,pos3);
    aux=((int)dosDecimales)%10;
    sprintf(message+ strlen(message), "%d", aux+48);
//    showChar(aux+48,pos4);
    return message;
}
*/
