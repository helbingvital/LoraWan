#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>

#define STATUS_MSG "$" // $ Asci = 0x24
//#define DATA_MSG 0xC0
#define DATA_MSG "C0"
#define USER "C"

char name[6];
char *synonym[1];

void main(void) {
    setUser('c');
    char test[10] = "AB"; //test = [65,66,...] "AB"
    uint16_t test_int = 996;
    sprintf(test+2, "%d", test_int);  //test = [65,66,57,57,54] "AB996" Ich will aber "AB03E4"
    
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
//    setup_gateway(synonym); 
    char status_msg[50]; 
    sprintf(status_msg, "AT+MSG=\"%sHello, LoRa! This is %s\"\r\n", STATUS_MSG, name);
    UART_send_string(status_msg);
    __delay_cycles(1000000);

    //Initialize Temperature Sensor and check connection
    ds18b20_init_pins();    
    int check = ds18b20_reset();
    if (check) {
        sprintf(status_msg, "AT+MSG=\"%s%s: Temperature Sensor is connected.\"\r\n", STATUS_MSG, synonym);
    } else {
        sprintf(status_msg, "AT+MSG=\"%s%s: Temperature Sensor is not connected.\"\r\n", STATUS_MSG, synonym);    
    }
    UART_send_string(status_msg);
    __delay_cycles(1000000);

    char hextest[50];
    sprintf(hextest, "AT+MSGHEX=\"%s%d\"\r\n", DATA_MSG, 336); //336 = 0x0150 -> 21 Celsius //macht C0336, dann padding mit 0 also 0C0336 weil keine halben bytes, daher ID falsch
    UART_send_string(hextest);
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
        // Nachricht über UART senden
        sprintf(message, "AT+MSGHEX=\"%s%d\"\r\n", DATA_MSG, sensorValue);
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
