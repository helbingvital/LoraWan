#include <LoraWAN2.h>
#include "msp430g2553.h"

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
    IE2 |= UCA0RXIE;            // RX-Interrupt aktivieren
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

void setup_gateway(char synonym){
//   selftest(NONE); //
    if (synonym == 'C') {
        UART_send_string("AT+KEY=APPKEY,\"B045041358B83A5E76F3C6312184CCC8\"\r\n"); // Set AppKey
    }
    if (synonym == 'O') {
        UART_send_string("AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n"); // Set AppKey
    }
    __delay_cycles(1000000);
    UART_send_string("AT+DR=EU868\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+CH=NUM,0-2\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+MODE=LWOTAA\r\n");
    __delay_cycles(1000000);
    UART_send_string("AT+JOIN\r\n");
    __delay_cycles(10000000);
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

