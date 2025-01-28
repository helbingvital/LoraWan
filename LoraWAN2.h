#ifndef LORAWAN_H
#define LORAWAN_H

#include <stdio.h>
#include <stdint.h>
#include <HAL_MSP430.h>
#include <string.h>



//Define Identifiers for message types. Should match the identifiers configured in decoder.
#define STATUS_MSG "$" // $ Asci = 0x24 = 36 (Decimal)
#define DATA_MSG "C0" // 0xC0 = 192 (Decimal)
#define MSG_TIMEOUT 10 //timeout in s
#define JOIN_TIMEOUT 15 //timeout in s
#define JOIN_TRIES 3

void LORA_TEST();
void UART_TXTEST();
void UART_RXTEST();
typedef enum{
    NONE,
    SEND,
    RECEIVE
} Mode;
void selftest(Mode mode);
unsigned int setup_gateway(char synonym);
uint16_t read_Sensor(void);
void setUser();
void send_Temperature(uint16_t sensorValue);
void handle_button_press(void);
void send_lora_text_msg(const char *message);
extern unsigned int send_status; //1 if busy, else 0
#endif
