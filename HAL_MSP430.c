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

}
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
    TA0CCTL0 |= CCIE;               // Enable Timer_A interrupt
    TA0CCR0 = ms*10;               // Set the timer value
    TA0CTL = TASSEL_2 + MC_1 + ID_3; // Select SMCLK, use up mode, use divider 8 (compatible to SMLCK if 1MHz)
}
//**********************************************************************************************************************************************************
// UART RX interrupt
//necessary to clear buffer to prevent overflow and maintain UART functionalities
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void) {
    char received_char = UCA0RXBUF;  // read received character and clear buffer
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
