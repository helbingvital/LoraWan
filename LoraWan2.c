#include <LoraWAN2.h>
#include "msp430g2553.h"

unsigned int send_status = 0; //0 send not in progress, 1 sent in progress
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
* Make sure to add your personal APPKEY
* Request is time out after JOIN_TIMEOUT seconds and retried JOIN_TRIES times.
*/
unsigned int setup_gateway(char synonym){
    const char *msg_done = "JOIN: Done"; 
    const char *msg_fail = "JOIN: Join failed"; 
    clear_uart_buffer();

    if (synonym == 'C') {
        UART_send_string("AT+KEY=APPKEY,\"B045041358B83A5E76F3C6312184CCC8\"\r\n"); // Set AppKey
    }
    if (synonym == 'O') {
        UART_send_string("AT+KEY=APPKEY,\"2B7E151628AED2A6ABF7158809CF4F3C\"\r\n"); // Set AppKey
    }
    UART_send_string("AT+DR=EU868\r\n");
    UART_send_string("AT+CH=NUM,0-2\r\n");
    UART_send_string("AT+MODE=LWOTAA\r\n");
    clear_uart_buffer();

    UART_send_string("AT+JOIN\r\n");
    int waited_time = 0;
    
    while (waited_time < JOIN_TIMEOUT) {
        DELAY_MS(1000);
        waited_time++;

        // Check if "MSG: Done" or "MSGHEX: Done" is in the buffer
        if (strstr(uart_buffer, msg_done)) {
            return 1;  // Exit if Join is succesfull
        }else if (strstr(uart_buffer, msg_fail)) {
            UART_send_string("AT+JOIN\r\n");
            continue;
        }
    }
    
    return 0;
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
 * Format SensorValue in order to send as Hex. Lookup Hex representation of each Nibble. Send Hex message to LoraChip via UART. Wait until processing is finished.
 *
 * @param sensorValue 16bit value. Bit 4-11 specify temperature, bit 12-15 specify fraction (1/16) (in accordance to ds18b20 specification)
 */
unsigned int send_Temperature(uint16_t sensorValue){
    clear_uart_buffer();
    const char *msghex_done = "MSGHEX: Done";
    const char *msg_disconnect = "MSGHEX: Please join network first";
    char hex_lookup[16] = "0123456789ABCDEF";

    // Format message
    char message[50];
    char char_3 = hex_lookup[(sensorValue & 0x000F)];
    sensorValue = sensorValue >> 4;
    char char_2 = hex_lookup[(sensorValue & 0x000F)];
    sensorValue = sensorValue >> 4;
    char char_1 = hex_lookup[(sensorValue & 0x000F)];
    int waited_time = 0;
    //If previous message isn't processed, wait for timeout and then continue anyways
    while (waited_time < MSG_TIMEOUT && send_status) {
        DELAY_US(100);
        waited_time++;

    }
    send_status = 1;
    sprintf(message, "AT+MSGHEX=\"%s0%c%c%c\"\r\n", DATA_MSG, char_1, char_2, char_3);
    UART_send_string(message);

    // Wait for response or timeout
    waited_time = 0;
    while (waited_time < MSG_TIMEOUT) {
        DELAY_MS(1000);
        waited_time++;

        // Check if "MSG: Done" or "MSGHEX: Done" is in the buffer
        if (strstr(uart_buffer, msghex_done)) {
            send_status = 0;
            return 1;  // Exit if response is received
        }else if (strstr(uart_buffer, msg_disconnect)) {
            setup_gateway(USER); //reconnect to Gateway
            send_status = 0; //give up on this try and return
            return 0;
        }
    }
    send_status = 0; //Send failed, still continue
    return 0;
}
//**********************************************************************************************************************************************************
/**
*Send message if button was pressed.
*/
void handle_button_press(void){
    char message[50];
    sprintf(message, "AT+MSG=\"%s%c:Button was pressed\"\r\n", STATUS_MSG, USER);
    send_lora_text_msg(message);
    button_press=0; 
    // UART_send_string(message);
}
//**********************************************************************************************************************************************************
unsigned int send_lora_text_msg(const char *message) {
    const char *msg_done = "MSG: Done";
    const char *msg_disconnect = "MSG: Please join network first";

    clear_uart_buffer(); 
    int waited_time = 0; 
    while (waited_time < MSG_TIMEOUT && send_status) { //Modem is busy with last message, wait.
        DELAY_US(100);
    }
    send_status = 1;
    UART_send_string(message);

    // Wait for response or timeout
    waited_time = 0;
    while (waited_time < MSG_TIMEOUT) {
        DELAY_MS(1000);
        waited_time++;

        // Check if "MSG: Done" or "MSGHEX: Done" is in the buffer
        if (strstr(uart_buffer, msg_done)) {
            send_status = 0; //set not busy
            return 1;  // Exit if response is received
        } else if (strstr(uart_buffer, msg_disconnect)) {
            setup_gateway(USER); //reconnect to Gateway
            send_status = 0; //give up on this try
            return 0;
        }
    }
    send_status = 0; //Send failed, still continue
    return 0;
}
