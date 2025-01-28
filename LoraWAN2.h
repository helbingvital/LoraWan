#ifndef LORAWAN_H
#define LORAWAN_H

#include <stdio.h>
#include <stdint.h>
#include <HAL_MSP430.h>
#include <string.h>



//Define Identifiers for message types. Should match the identifiers configured in decoder.
#define STATUS_MSG "$" // $ Asci = 0x24 = 36 (Decimal)
#define DATA_MSG "C0" // 0xC0 = 192 (Decimal)


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
void handle_button_press(void);
void send_lora_text_msg(const char *message);
extern unsigned int send_status;
#endif
