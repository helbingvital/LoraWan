#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>


char *name[6];
char *synonym[1];

void main(void) {
    setUser('c');

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

    /*Getting Started: 
    Use selftest(NONE) to get ID, if you have 2 devices use selftest(SEND) and selftest(RECEIVE) to check functionality
    selftest(SEND);*/

    //Join Lora Gateway. Only necessary if disconnected. Send Status/Hello Message to Network.
    setup_gateway(synonym); 
    char tempC[50]; 
    sprintf(tempC, "AT+MSG=\"Hello, LoRa! This is %s\"\r\n", name);
    UART_send_string(tempC);
    __delay_cycles(1000000);

    //Initialize Temperature Sensor and check connection
    ds18b20_init_pins();    
    int check = ds18b20_reset();
    if (check) {
        sprintf(tempC, "AT+MSG=\"%s: Temperature Sensor is connected.\"\r\n", synonym);
    } else {
        sprintf(tempC, "AT+MSG=\"%s: Temperature Sensor is not connected.\"\r\n", synonym);    
    }
    UART_send_string(tempC);
    P1OUT |= DS18B20_POWER_PIN; //power on
    uint16_t sensorValue = 0;

    //Main Loop: Reoccurent Temperature Measurements sent via LoraWan
    while (1) {
        // Optionale Verarbeitung empfangener Daten oder Fehlerbehandlung
        sensorValue = ds18b20_get_temp();
        //Verarbeitung: Rechtshift 4bit, vernachlaessigt nachkommastelle. 
        //Dann AND operation mit 0000 0000 1111 1111 um nur die letzten 8 bit zu betrachten
        sensorValue = sensorValue >> 4;
        sensorValue &= 0x00FF;
        // Format message
        char message[50];
        sprintf(message, "AT+MSG=\"%s: Sensor Value: %d\"\r\n",synonym,sensorValue);
        // Nachricht über UART senden
        UART_send_string(message);
         __delay_cycles(10000000);

        // Toggle LED to visualize end of iteration.
        P1OUT ^= LED;                                
    }
}

void setUser(char input) {
    // Switch-Case-Abfrage
    switch (input) {
        case 'c':
            // Parameter für 'c' setzen
            sprintf(synonym, "C");
            sprintf(name, "Celina");
            break;
        case 'o':
            // Parameter für 'o' setzen
            sprintf(synonym, "O");
            sprintf(name, "Oliver");
            break;
    }
}