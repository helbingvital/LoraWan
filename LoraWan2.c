#include <LoraWAN2.h>
#include "msp430g2553.h"

//**********************************************************************************************************************************************************
void UART_TXTEST() {
    // Set test mode
    UART_send_string("AT+MODE=TEST\r\n");
    // Query test mode, check RF configuration
    UART_send_string("AT+TEST=?\r\n");
    // Modify to set RF Configuration
    UART_send_string("AT+TEST=RFCFG,[F],[SF],[BW],[TXPR],[RXPR],[POW],[CRC],[IQ],[NET]\r\n");
    // Send HEX format packet
    // UART_send_string("AT+TEST=TXLRPKT, "00 AA 11 BB 22 CC"\r\n");
    // Send TEXT format packet
    UART_send_string("AT+TEST=TXLRSTR,\"LoRaWAN Modem\"\r\n");
}
//**********************************************************************************************************************************************************
void UART_RXTEST() {
    // Set test mode
    UART_send_string("AT+MODE=TEST\r\n");
    // Query test mode, check RF configuration
    UART_send_string("AT+TEST=?\r\n");
    // Set RF Configuration
//    UART_send_string("AT+TEST=RFCFG,[FREQUENCY],[SF],[BANDWIDTH],[TXPR],[RXPR],[POW],[CRC],[IQ],[NET]\r\n");
    // Enter RX continuous mode
    UART_send_string("AT+TEST=RXLRPKT\r\n");
}
//**********************************************************************************************************************************************************
/** 
* Join Lora Gateway. 
*
* Make sure to add your personal APPKEY*/
void setup_gateway(char synonym){
    if (synonym == 'C') {
        UART_send_string("AT+KEY=APPKEY,\"B045041358B83A5E76F3C6312184CCC8\"\r\n"); // Set AppKey
    }
    if (synonym == 'O') {
        UART_send_string("AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n"); // Set AppKey
    }
    UART_send_string("AT+DR=EU868\r\n");
    UART_send_string("AT+CH=NUM,0-2\r\n");
    UART_send_string("AT+MODE=LWOTAA\r\n");
    UART_send_string("AT+JOIN\r\n");
    DELAY_MS(7000);
}
//**********************************************************************************************************************************************************
/**
 * Test Lora module
 *
 * Request Version, DevAddr(ABP), DevEui(OTAA) and AppEui(OTAA) from module via UART. Option to switch module into test mode.
 * @param mode Used to specify if Transmitter (SEND), Receiver (RECEIVER) or nothing (NONE) should be tested additionally
 */
void selftest(Mode mode){
// Sende AT-Befehl an Wio E5
    UART_send_string("AT\r\n");            // Send "AT" 
    UART_send_string("AT+VER?\r\n");       // Get Version
    UART_send_string("AT+ID?\r\n");      // Get DevAddr(ABP), DevEui(OTAA), AppEui(OTAA)
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

//**********************************************************************************************************************************************************
/**
 * Format and send SensorValue (from ds18b20 Temperature Sensor) 
 *
 * Format SensorValue in order to send as Hex. Lookup Hex representation of each Nibble. Send Hex message to LoraChip via UART.
 *
 * @param sensorValue 16bit value. Bit 4-11 specify temperature, bit 12-15 specify fraction (1/16) (in accordance to ds18b20 specification)
 */
void send_Temperature(uint16_t sensorValue){
    char hex_lookup[16] = "0123456789ABCDEF";
    // Format message
    char message[50];
    char char_3 = hex_lookup[(sensorValue & 0x000F)];
    sensorValue = sensorValue >> 4;
    char char_2 = hex_lookup[(sensorValue & 0x000F)];
    sensorValue = sensorValue >> 4;
    char char_1 = hex_lookup[(sensorValue & 0x000F)];

    sprintf(message, "AT+MSGHEX=\"%s0%c%c%c\"\r\n", DATA_MSG, char_1, char_2, char_3);
    UART_send_string(message);
}
//**********************************************************************************************************************************************************
/**
*Send message if button was pressed.
*/
void handle_button_press(void){
    char message[50];
    sprintf(message, "AT+MSG=\"%s%c:Button was pressed\"\r\n", STATUS_MSG, USER);
    UART_send_string(message);
}
//**********************************************************************************************************************************************************
