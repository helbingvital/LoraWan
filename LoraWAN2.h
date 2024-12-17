#include <stdio.h>
#include <stdint.h>

//Define pins according to your microcontroller hardware
#define LED BIT0


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

/**
 * @brief Test Lora modula
 * Request Version, DevAddr(ABP), DevEui(OTAA) and AppEui(OTAA) from module via UART. Option to switch module into test mode.
 * @param mode Used to specify if Transmitter (SEND), Receiver (RECEIVER) or no (NONE) should be tested additionally
 */
void selftest(Mode mode);
void setup_gateway(char* synonym);

uint16_t read_Sensor(void);
void setUser(char input);

