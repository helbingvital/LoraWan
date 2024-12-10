#include <stdio.h>
#include <stdint.h>

//#include <cstdint>
void init_UART(void);
void UART_send_string(const char *str);
void UART_send_char(char c);
void UART_TXTEST();
void UART_RXTEST();
typedef enum{
    NONE,
    SEND,
    RECEIVE
} Mode;
void selftest(Mode mode);
void setup_gateway();

void init_Sensor(void);
uint16_t read_Sensor(void);
void setModem(char input);

/*
// 1-Wire Protokoll: Delay-Funktion
void delay_us(unsigned int us);
// GPIO Konfiguration
void init_pins();
// DS18B20: Reset Impuls
unsigned char ds18b20_reset();
// DS18B20: Schreibe ein Bit
void ds18b20_write_bit(unsigned char bit);
// DS18B20: Lese ein Bit
unsigned char ds18b20_read_bit();
// DS18B20: Schreibe ein Byte
void ds18b20_write_byte(unsigned char byte);
// DS18B20: Lese ein Byte
unsigned char ds18b20_read_byte();
// DS18B20: Temperatur messen
float ds18b20_get_temperature();
*/
