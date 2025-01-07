#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>

#define STATUS_MSG "$" // $ Asci = 0x24
#define DATA_MSG "C0"
#define USER 'C'

char name[6];
char *synonym[1];

void main(void) {
    setUser();
    char hex_lookup[16] = "0123456789ABCDEF";
    
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
    __delay_cycles(1000000);

    //Initialize Temperature Sensor and check connection
    ds18b20_init_pins();    
    int check = ds18b20_reset();
    if (check) {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is connected.\"\r\n", STATUS_MSG, USER);
    } else {
        sprintf(status_msg, "AT+MSG=\"%s%c: Temperature Sensor is not connected.\"\r\n", STATUS_MSG, USER);    
    }
    UART_send_string(status_msg);
    __delay_cycles(1000000);
    
    P1OUT |= DS18B20_POWER_PIN; //power on
    uint16_t sensorValue = 0;
    uint16_t sensorValue_temp = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    while (1) {
        // Optionale Verarbeitung empfangener Daten oder Fehlerbehandlung
        sensorValue = ds18b20_get_temp();
        //Verarbeitung: Rechtshift 4bit, vernachlaessigt nachkommastelle. 
        //Dann AND operation mit 0000 0000 1111 1111 um nur die letzten 8 bit zu betrachten
        sensorValue_temp = sensorValue; // >> 4;
        sensorValue_temp &= 0x00FF;
        // Format message
        char message[50];
//        sprintf(message, "AT+MSG=\"%s: Sensor Value: %d\"\r\n",synonym,sensorValue);
        char char_3 = hex_lookup[(sensorValue & 0x000F)];
        sensorValue = sensorValue >> 4;
        char char_2 = hex_lookup[(sensorValue & 0x000F)];
        sensorValue = sensorValue >> 4;
        char char_1 = hex_lookup[(sensorValue & 0x000F)];

        sprintf(message, "AT+MSGHEX=\"%s0%c%c%c\"\r\n", DATA_MSG, char_1, char_2, char_3);
        UART_send_string(message);
         __delay_cycles(10000000);

        // Toggle LED to visualize end of iteration.
        P1OUT ^= LED;                                
    }
}

void setUser() {
    // Switch-Case-Abfrage
    switch (USER) { 
        case 'C':
            sprintf(name, "Celina");
            break;
        case 'O':
            sprintf(name, "Oliver");
            break;
    }
}
