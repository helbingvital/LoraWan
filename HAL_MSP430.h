#ifndef HAL_MSP430_H
#define HAL_MSP430_H

#include "msp430g2553.h"
#include <msp430.h>

#define USER 'C'

//Define pins according to your microcontroller hardware
#define LED1 BIT0
#define LED1DIR P1DIR 
#define LED_OUT P1OUT //Use LED at Pin 1.0

// Configure registers used for button (Pin, Port, and registers)
#define BUTTON_PIN BIT3
#define BUTTON_PDIR P1DIR //Button is connected to P1.3
#define BUTTON_POUT P1OUT
#define BUTTON_PREN P1REN //ensure to use P1... registers for button 
#define BUTTON_PINTERRUPT P1IE
#define BUTTON_PTRIGGER P1IES 
#define BUTTON_PIFG P1IFG //interrupt flag

//Configuration for UART, connected to P1.1 and P1.2
#define UART_RX BIT1 
#define UART_TX BIT2
#define BAUD 104 // Baudrate 9600 (1 MHz / 9600)

#define UART_BUFFER_SIZE 128
extern char uart_buffer[UART_BUFFER_SIZE];  // UART buffer for storing received data
extern int buffer_index;          // Buffer index for tracking position

//Temperature Sensor
// DQ-Pin und Power-Pin definieren for temperature sensor
#define DQ_PIN BIT4 // P1.4
// Defines to configure pins. Choose according to your hardware.
#define DS18B20_PORT_DIR P1DIR //used Port for temperature Sensor
#define DS18B20_PORT_OUT P1OUT //output port
#define DS18B20_PORT_IN  P1IN //input port
#define DS18B20_PORT_REN P1REN 
#define DS18B20_PORT_PIN BIT4 //data pin
#define DS18B20_POWER_PIN BIT5 // P1.5

void init_UART(void);
void init_button();
void UART_send_string(const char *str);
void UART_send_char(char c);
void init_LED();
void init_microcontroller();
void setup_timer(int ms);
void clear_uart_buffer(void);

extern unsigned int timer_flag;


#endif //HAL_MSP430

//Define Delays
#ifndef DELAY_H_
#define DELAY_H_

#define CYCLES_PER_US 1 // depends on clock speed, 1 for 1MHZ
#define CYCLES_PER_MS (CYCLES_PER_US * 1000UL)

#define DELAY_US(x) __delay_cycles((x * CYCLES_PER_US))
#define DELAY_MS(x) __delay_cycles((x * CYCLES_PER_MS))

#endif /* DELAY_H_ */

#ifndef STRING_FCT
#define STRING_FCT
int strings_equal(const char *str1, const char *str2);
int substring_in_buffer(const char *buffer, const char *substring);
#endif
