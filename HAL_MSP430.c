#include <HAL_MSP430.h>
#include <LoraWAN2.h>
/**
* Initialize MSP 430
*
*Stop Watchdogtimer and initialize clock
*/
void init_microcontroller(){
    //Set Up Microcontroller
    WDTCTL = WDTPW | WDTHOLD;  // Watchdog-Timer stoppen

    // Clockconfiguration, Use 1 MHZ clock
    if (CALBC1_1MHZ==0xFF) {   // check if internal clock calibrated
        while(1);               
    }
    DCOCTL = 0;                 
    BCSCTL1 = CALBC1_1MHZ;      
    DCOCTL = CALDCO_1MHZ;       // set DCO to 1 MHz

    timer_flag = 0;

}
//**********************************************************************************************************************************************************
unsigned int timer_flag = 0;
//**********************************************************************************************************************************************************
/** 
* Configure UART on MSP430
*/
void init_UART(void) {
    
    P1SEL |= UART_RX + UART_TX;       
    P1SEL2 |= UART_RX + UART_TX;

    UCA0CTL1 |= UCSSEL_2;       // SMCLK as clock
    UCA0BR0 = BAUD;              //Set Baudrate, UCA0BR0 = Clockfreq/Baudrate
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS0;          // Modulation for 9600 baud
    UCA0CTL1 &= ~UCSWRST;       // get USCI from reset 
    IE2 |= UCA0RXIE;            // activate RX-Interrupt 
}
//**********************************************************************************************************************************************************
/**
* Configure Microcontroller to support interrupt routine on press of button
*/
void init_button(){
    BUTTON_PDIR &= ~BUTTON_PIN;             // Set P1.3 as input
    BUTTON_PREN |= BUTTON_PIN;              // Enable pull-up/down resistor on P1.3
    BUTTON_POUT |= BUTTON_PIN;              // Select pull-up resistor

    // Enable interrupts
    BUTTON_PINTERRUPT |= BUTTON_PIN;        // Enable interrupt
    // BUTTON_PINTERRUPT = 0x00;
    BUTTON_PTRIGGER |= BUTTON_PIN;          // Trigger on high-to-low transition (button press)
    BUTTON_PIFG &= ~BUTTON_PIN;             // Clear interrupt flag 

}
//**********************************************************************************************************************************************************
/** 
* Send string via UART and wait 1s
*/
void UART_send_string(const char *str) {
    while (*str) {
        UART_send_char(*str++);
    }
    DELAY_MS(3000);
}
//**********************************************************************************************************************************************************
/** 
* Send single character via UART*/
void UART_send_char(char c) {
    while (!(IFG2 & UCA0TXIFG));  // Wait until TX-Buffer empty
    UCA0TXBUF = c;                // Write
}
//**********************************************************************************************************************************************************
/** 
* Set LED Pin to output direction*/
void init_LED(){
     LED1DIR |= LED1;
}
//**********************************************************************************************************************************************************
/** 
* Configure Timer Module on MSP430 and start timer
*
* Can be used instead of DELAY(MS), calls interrupt after ms
*/
void setup_timer(int ms) {
    timer_flag=0;
    TA0CCTL0 |= CCIE;               // Enable Timer_A interrupt
    TA0CCR0 = ms*10;               // Set the timer value
    TA0CTL = TASSEL_2 + MC_1 + ID_3; // Select SMCLK, use up mode, use divider 8 (compatible to SMLCK if 1MHz)
}
//**********************************************************************************************************************************************************
char uart_buffer[UART_BUFFER_SIZE] = {0};  // UART buffer for received data
int buffer_index = 0;            // Buffer index
//**********************************************************************************************************************************************************
void clear_uart_buffer(void) {
    int i = 0;
    for (i=0; i < UART_BUFFER_SIZE; i++){
        uart_buffer[i] = '\0';
    }    
    buffer_index = 0;
}
//**********************************************************************************************************************************************************
// UART RX interrupt
//necessary to clear buffer to prevent overflow and maintain UART functionalities
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    // if (UCA0IFG & UCRXIFG) {
        char received_char = UCA0RXBUF;  // read received character and clear buffer
        if (received_char == '+'){
            clear_uart_buffer();
        }
        if (buffer_index < UART_BUFFER_SIZE - 1) {
            uart_buffer[buffer_index++] = received_char;
            uart_buffer[buffer_index] = '\0'; // Null-terminate the string
        }
    // }
}
//**********************************************************************************************************************************************************
/**
*Interrupt Routine for Button Press
*
*Call function to take action on Button press and clear flag.
*/
#pragma vector=PORT1_VECTOR 
__interrupt void Port_1_ISR(void) {
    if (BUTTON_PIFG & BUTTON_PIN) {
        handle_button_press();  
        BUTTON_PIFG &= ~BUTTON_PIN;
    }
}
//**********************************************************************************************************************************************************
// Timer ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    timer_flag = 1;                // Set the flag to indicate timer elapsed
}


int strings_equal(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1 != *str2) {
            return 0;  // Strings are not equal
        }
        str1++;
        str2++;
    }
    return (*str1 == '\0' && *str2 == '\0');  // Ensure both strings end at the same time
}

int substring_in_buffer(const char *buffer, const char *substring) {
    while (*buffer) {
        const char *b = buffer;
        const char *s = substring;

        // Compare substring with part of the buffer
        while (*b && *s && (*b == *s)) {
            b++;
            s++;
        }

        // If we reached the end of the substring, it's a match
        if (*s == '\0') {
            return 1;  // Substring found
        }

        buffer++;
    }
    return 0;  // Substring not found
}
