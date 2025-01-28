
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
    //JOIN: Join failed
    // JOIN: Done
    char status_msg[50]; 
    sprintf(status_msg, "AT+MSG=\"%sHello, LoRa! This is %s\"\r\n", STATUS_MSG, name);
    // UART_send_string(status_msg);
    send_lora_text_msg(status_msg);

    //Initialize Temperature Sensor and check connection
    ds18b20_init_pins();    
    if (ds18b20_reset()) {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is connected.\"\r\n", STATUS_MSG, USER);
    } else {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is not connected.\"\r\n", STATUS_MSG, USER);    
    }
    DELAY_MS(8000);
    // UART_send_string(status_msg);
    send_lora_text_msg(status_msg);

    DS18B20_PORT_OUT |= DS18B20_POWER_PIN; //temperature sensor: power on
    uint16_t sensorValue = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    while (1) {
        sensorValue = ds18b20_get_temp();
        send_Temperature(sensorValue);
        // Toggle LED to visualize end of iteration.
        LED_OUT ^= LED1;
        DELAY_MS(8000);         
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

