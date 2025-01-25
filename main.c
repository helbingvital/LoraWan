
#include <LoraWAN2.h>
#include <ds18b20.h>

#define MSP430
#ifdef MSP430
#include <HAL_MSP430.h>
//#include "msp430g2553.h"
//#include <msp430.h>
#else
//include Hardware Abstraction Layer for other Microcontroller
#endif

#define SETUP_MODE 0 //1 if you want to get DevEui and AppEui
char name[6];
volatile unsigned int timer_flag = 0;  // Flag to indicate timer interrupt

void main(void) {
    setUser();
    
    init_microcontroller();     //initialize microcontroller
    init_UART();                // initialize UART 
    init_button();              //initialize Button and interrupt
    init_LED();                 //initialize LED
    __enable_interrupt();

//    Getting Started: 
#if SETUP_MODE
    selftest(NONE);
#else
    //Join Lora Gateway. Only necessary if disconnected.
    setup_gateway(USER); 

    char status_msg[50]; 
    sprintf(status_msg, "AT+MSG=\"%sHello, LoRa! This is %s\"\r\n", STATUS_MSG, name);
    UART_send_string(status_msg);

    //Initialize Temperature Sensor and check connection
    ds18b20_init_pins();    
    if (ds18b20_reset()) {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is connected.\"\r\n", STATUS_MSG, USER);
    } else {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is not connected.\"\r\n", STATUS_MSG, USER);    
    }
    UART_send_string(status_msg);
    
    DS18B20_PORT_OUT |= DS18B20_POWER_PIN; //temperature sensor: power on
    uint16_t sensorValue = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    setup_timer(1);
    while (1) {
        if(timer_flag){
            sensorValue = ds18b20_get_temp();
            send_Temperature(sensorValue);
        //    DELAY_MS(5000);
        //    __bis_SR_register(LPM0_bits + GIE);
            // Toggle LED to visualize end of iteration.
            LED_OUT ^= LED1;    
            setup_timer(5);            
        }
        __bis_SR_register(LPM0_bits + GIE);                
    }
#endif
}

void setUser() {
    switch (USER) { 
        case 'C':
            sprintf(name, "Celina");
            break;
        case 'O':
            sprintf(name, "Oliver");
            break;
    }
}

#pragma vector=PORT1_VECTOR //move to hardware config.c
__interrupt void Port_1_ISR(void) {
    if (BUTTON_PIFG & BUTTON_PIN) {
        handle_button_press();  
        BUTTON_PIFG &= ~BUTTON_PIN;
        __bic_SR_register_on_exit(LPM0_bits);
    }
}

// Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    timer_flag = 1;                // Set the flag to indicate 5 seconds elapsed
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 to allow main loop to run
}
