#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>
#include <hardware_config.h>


#define USER 'C'
#define SETUP_MODE 0 //1 if you want to get DevEui and AppEui

char name[6];

void main(void) {
    setUser();
    
    //Set Up Microcontroller
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog-Timer stoppen

    // Clockconfiguration, Use 1 MHZ clock
    if (CALBC1_1MHZ==0xFF) {   // check if internal clock calibrated
        while(1);               
    }
    DCOCTL = 0;                 
    BCSCTL1 = CALBC1_1MHZ;      
    DCOCTL = CALDCO_1MHZ;       // set DCO to 1 MHz

    init_UART();                // initialize UART 
    LED1DIR |= LED1;               // Set LED pin to output direction

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
    int check = ds18b20_reset();
    if (check) {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is connected.\"\r\n", STATUS_MSG, USER);
    } else {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is not connected.\"\r\n", STATUS_MSG, USER);    
    }
    UART_send_string(status_msg);
    
    P1OUT |= DS18B20_POWER_PIN; //temperature sensor: power on
    uint16_t sensorValue = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    while (1) {
        sensorValue = ds18b20_get_temp();
        send_Temperature(sensorValue);
        // Toggle LED to visualize end of iteration.
        P1OUT ^= LED1;                                
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
