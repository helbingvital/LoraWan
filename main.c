#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>


#define USER 'C'
char name[6];

void main(void) {
    setUser();
    
    //Set Up Microcontroller
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog-Timer stoppen
    // Taktkonfiguration (falls notwendig, z.B. auf 1 MHz setzen)
    if (CALBC1_1MHZ==0xFF) {   // Wenn keine Kalibrierwerte vorhanden sind
        while(1);               // Hängen bleiben, da nicht kalibriert
    }
    DCOCTL = 0;                 
    BCSCTL1 = CALBC1_1MHZ;      // Setze DCO auf 1 MHz
    DCOCTL = CALDCO_1MHZ;

    init_UART();                // UART initialisieren
    P1DIR |= LED;                            // Set LED pin to output direction

//    Getting Started: 
//    selftest(NONE);
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
    
    P1OUT |= DS18B20_POWER_PIN; //power on
    uint16_t sensorValue = 0;
    uint16_t sensorValue_temp = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    while (1) {
        sensorValue = ds18b20_get_temp();
        send_Temperature(sensorValue);
        // Toggle LED to visualize end of iteration.
        P1OUT ^= LED;                                
    }
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
