#include "msp430g2553.h"

#include <msp430.h>
#include <LoraWAN2.h>
#include <ds18b20.h>

 /*
void setup_gateway(void);
void init_UART(void);
void UART_send_string(const char *str);
void UART_send_char(char c);
void UART_RXTEST();
void UART_TXTEST();
void selftest(Mode mode); 
*/

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog-Timer stoppen

    // Taktkonfiguration (falls notwendig, z.B. auf 1 MHz setzen)
    if (CALBC1_1MHZ==0xFF) {   // Wenn keine Kalibrierwerte vorhanden sind
        while(1);               // Hängen bleiben, da nicht kalibriert
    }
    DCOCTL = 0;                 
    BCSCTL1 = CALBC1_1MHZ;      // Setze DCO auf 1 MHz
    DCOCTL = CALDCO_1MHZ;

    init_UART();                // UART initialisieren

    //LEDs
    P1DIR |= BIT6;                            // Set P1.6 (BIT6) to output direction
    P1DIR |= BIT0;

//Getting Started: use selftest(NONE) to get ID, if you have 2 devices use selftest(SEND) and selftest(RECEIVE) to check functionality
//    selftest(SEND);
//   setup_gateway(); //Ausführen wenn Microcontroller neu gestartet ist.

//Send Data
    UART_send_string("AT+MSG=\"Hello, LoRa! This is Celina\"\r\n"); // Sendet eine Nachricht über LoRa
    __delay_cycles(1000000);

//Sensor
    init_pins();
    P1OUT |= POWER_PIN; //power on
    int check = ds18b20_reset();
    if (check){
        UART_send_string("AT+MSG=\"C: Temperature Sensor is connected.\"\r\n");
    }else{
        UART_send_string("AT+MSG=\"C: Temperature Sensor not is connected.\"\r\n");

    }


    uint16_t sensorValue = 0;
    // int temp = 0;
    while (1) {
        // Optionale Verarbeitung empfangener Daten oder Fehlerbehandlung
        sensorValue = ds18b20_get_temp();
        //Verarbeitung: Rechtshift 4bit, vernachlaessigt nachkommastelle. 
        //Dann AND operation mit 0000 0000 1111 1111 um nur die letzten 8 bit zu betrachten
        sensorValue = sensorValue >> 4;
        sensorValue &= 0x00FF;
        // Nachricht formatieren
        char message[50];              // Puffer für die formatierte Nachricht
        //sprintf(message, "AT+MSG=\"C: Sensor Value: %d \r\n", temp);
       
        sprintf(message, "AT+MSG=\"%d\"\r\n", sensorValue);
        // Nachricht über UART senden
        UART_send_string(message);
         __delay_cycles(10000000);

        P1OUT ^= BIT0;                        // Toggle P1.0 using exclusive-OR        

    }
}

void init_UART(void) {
    // Setze Pins P1.1 und P1.2 als UART
    P1SEL |= BIT1 + BIT2;       // P1.1 = RX, P1.2 = TX
    P1SEL2 |= BIT1 + BIT2;

    // Konfiguration des UARTs
    UCA0CTL1 |= UCSSEL_2;       // SMCLK als Taktquelle
    UCA0BR0 = 104;              // Baudrate 9600 (1 MHz / 9600)
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;          // Modulation für 9600 Baud
    UCA0CTL1 &= ~UCSWRST;       // USCI aus Reset holen
    IE2 |= UCA0RXIE;            // RX-Interrupt aktivieren (optional)
}

void UART_send_string(const char *str) {
    while (*str) {
        UART_send_char(*str++);
    }
}

void UART_send_char(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Warten, bis TX-Buffer leer ist
    UCA0TXBUF = c;                // Zeichen senden
}

// Optional: ISR für empfangene Zeichen (z.B. um auf die Antwort des Moduls zu warten)
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    char received_char = UCA0RXBUF;  // Lese empfangenes Zeichen
    // Hier können Sie empfangene Daten verarbeiten oder speichern
}

void UART_TXTEST() {
    // Set test mode
    UART_send_string("AT+MODE=TEST\r\n");
    // Query test mode, check RF configuration
    UART_send_string("AT+TEST=?\r\n");
    // Set RF Configuration
    UART_send_string("AT+TEST=RFCFG,[F],[SF],[BW],[TXPR],[RXPR],[POW],[CRC],[IQ],[NET]\r\n");
    // Send HEX format packet
    // UART_send_string("AT+TEST=TXLRPKT, "00 AA 11 BB 22 CC"\r\n");
    // Send TEXT format packet
    UART_send_string("AT+TEST=TXLRSTR,\"LoRaWAN Modem\"\r\n");
}

void UART_RXTEST() {
    // Set test mode
    UART_send_string("AT+MODE=TEST\r\n");
    __delay_cycles(1000000);
    // Query test mode, check RF configuration
    UART_send_string("AT+TEST=?\r\n");
    __delay_cycles(1000000);
    // Set RF Configuration
//    UART_send_string("AT+TEST=RFCFG,[FREQUENCY],[SF],[BANDWIDTH],[TXPR],[RXPR],[POW],[CRC],[IQ],[NET]\r\n");
    // Enter RX continuous mode
    UART_send_string("AT+TEST=RXLRPKT\r\n");
    __delay_cycles(1000000);

}

void setup_gateway(void){
//    Mode test = 0;   
//    selftest(NONE);
    UART_send_string("AT+KEY=APPKEY,\"B045041358B83A5E76F3C6312184CCC8\"\r\n"); // Set AppKey
    __delay_cycles(1000000);
    UART_send_string("AT+DR=EU868\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+CH=NUM,0-2\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+MODE=LWOTAA\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+JOIN\r\n");
    __delay_cycles(8000000);
}

void selftest(Mode mode){
// Sende AT-Befehl an Wio E5
    UART_send_string("AT\r\n");            // Sende "AT" und ein Zeilenumbruch
    UART_send_string("AT+VER?\r\n");       // Befehl zur Versionsabfrage
    UART_send_string("AT+ID?\r\n");      // Read all, DevAddr(ABP), DevEui(OTAA), AppEui(OTAA)
    __delay_cycles(1000000);
    switch (mode){
        case SEND:
            UART_TXTEST();
            break;
        case RECEIVE:
            UART_RXTEST();
            break;
        case NONE:
          break;
        }
}

void init_Sensor(void){
    // Pin 1.3 als Eingang konfigurieren
    P1DIR &= ~BIT3;  // Setze Bit 3 von P1DIR auf 0 (Input)
    P1REN |= BIT3;   // Aktiviere Pull-Up/Pull-Down-Widerstand
    P1OUT |= BIT3;   // Wähle Pull-Up-Widerstand (falls benötigt)
}

uint16_t read_Sensor(void){
    uint16_t pinValue = 0; // Variable für den Zustand von P1.3
    pinValue = P1IN;   
    return  pinValue;
}

// 1-Wire Protokoll: Delay-Funktion
void delay_us(unsigned int us) {
    while (us--) {
        __delay_cycles(1); // Anpassung je nach Taktrate des MSP430
    }
}


/*

// DS18B20: Schreibe ein Bit
void ds18b20_write_bit(unsigned char bit) {
    P1OUT &= ~DQ_PIN;       // Ziehe DQ auf Low
    P1DIR |= DQ_PIN;        // DQ als Ausgang
    if (bit) {
        delay_us(10);       // Halte Low für 10µs
        P1OUT |= DQ_PIN;    // Setze DQ auf High
        delay_us(55);       // Halte High für 55µs
    } else {
        delay_us(65);       // Halte Low für 65µs
        P1OUT |= DQ_PIN;    // Setze DQ auf High
        delay_us(5);
    }
}

// DS18B20: Lese ein Bit
unsigned char ds18b20_read_bit() {
    unsigned char bit = 0;
    P1OUT &= ~DQ_PIN;       // Ziehe DQ auf Low
    P1DIR |= DQ_PIN;        // DQ als Ausgang
    delay_us(3);            // Halte für 3µs
    P1DIR &= ~DQ_PIN;       // Schalte DQ auf Eingang
    delay_us(10);           // Warte 10µs
    bit = (P1IN & DQ_PIN) ? 1 : 0; // Lese Wert
    delay_us(50);           // Halte für 50µs
    return bit;
}

// DS18B20: Schreibe ein Byte
void ds18b20_write_byte(unsigned char byte) {
    int i = 0;
    for (i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

// DS18B20: Lese ein Byte
unsigned char ds18b20_read_byte() {
    unsigned char byte = 0;
    int i = 0;
    for (i = 0; i < 8; i++) {
        byte |= (ds18b20_read_bit() << i);
    }
    return byte;
}
*/

/*
// DS18B20: Temperatur messen
float ds18b20_get_temperature() {
    unsigned char temp_lsb, temp_msb;
    float temp;
    
    ds18b20_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0x44); // Start Conversion
    __delay_cycles(800000);   // Warte ca. 750ms

    ds18b20_reset();
    ds18b20_write_byte(0xCC); // Skip ROM
    ds18b20_write_byte(0xBE); // Read Scratchpad

    temp_lsb = ds18b20_read_byte();
    temp_msb = ds18b20_read_byte();

    temp = (temp_msb << 8) | temp_lsb; // Kombiniere MSB und LSB
    return temp * 0.0625; // Temperatur in °C
}
*/
