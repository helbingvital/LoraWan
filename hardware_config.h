#include "msp430g2553.h"

//Define pins according to your microcontroller hardware
#define LED1 BIT0
#define LED1DIR P1DIR //Use LED at Pin 1.0

#define UART_RX BIT1 //Pins for UART
#define UART_TX BIT2
#define BAUD 104 // Baudrate 9600 (1 MHz / 9600)

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


//Define Delays
#ifndef DELAY_H_
#define DELAY_H_

#define CYCLES_PER_US 1 // depends on clock speed, 1 for 1MHZ
#define CYCLES_PER_MS (CYCLES_PER_US * 1000UL)

#define DELAY_US(x) __delay_cycles((x * CYCLES_PER_US))
#define DELAY_MS(x) __delay_cycles((x * CYCLES_PER_MS))

#endif /* DELAY_H_ */
