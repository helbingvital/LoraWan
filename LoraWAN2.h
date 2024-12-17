#include <stdio.h>
#include <stdint.h>

//Define Hadware connection points according to your hardware
#define LED BIT0


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
void setUser(char input);

