#ifndef LORAWAN_H
#define LORAWAN_H

#include <stdio.h>
#include <stdint.h>
#include <hardware_config.h>



//Define Identifiers for message types. Should match the identifiers configured in decoder.
#define STATUS_MSG "$" // $ Asci = 0x24 = 36 (Decimal)
#define DATA_MSG "C0" // 0xC0 = 192 (Decimal)

void init_UART(void);
void UART_send_string(const char *str);
void UART_send_char(char c);
void LORA_TEST();
void UART_TXTEST();
void UART_RXTEST();
typedef enum{
    NONE,
    SEND,
    RECEIVE
} Mode;
void selftest(Mode mode);
void setup_gateway(char synonym);
uint16_t read_Sensor(void);
void setUser();
void send_Temperature(uint16_t sensorValue);

#endif
