#include <msp430.h> 

int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    UCSCTL4 = SELA_0;           //

    //P1SEL &= ~(BIT0 + BIT3);    // Selects I/O function
    //P1SEL2 &= ~(BIT0 + BIT3);   // Selects I/O function

    //LED
    P1DIR |= BIT0;              // Sets P1.0 in the output direction
    P1OUT &= ~BIT0;             // Clears P1.0 output register value


    //BUTTON
    P1DIR &= ~BIT1;             // Sets P1.1 in the input direction
    P1REN |= BIT1;              // P1.1 pullup/pulldown resistor enabled
    P1OUT |= BIT1;              // P1.1 pullup/pulldown configured as
                                // pull up resistor

    P1IE |= BIT1;               // P1.1 interrupt enabled
    P1IES |= BIT1;              // P1.1 interrupt flag is set with a high
                                // to low transition
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared

    TA0CCTL0 |= CCIE;           // Capture/compare interrupt enabled
    TA0CCR0 = 0x21;             // Sets the compare register to be triggered
                                // every 1 ms

    _BIS_SR(LPM0_bits + GIE);   // Sets the processor to low processor mode
                                // and enables global interrupts
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void button_interrupt(void){
    TA0CTL = TASSEL_2 + MC_2 + TACLR;
                                // Sets timer to A clock, sets the clock to
                                // continuous mode and clears the clock
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared
    P1IES ^= BIT1;              // Toggles the edge triggers the interrupt
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void timer0_a_interrupt(void){
    if (!(P1IN & BIT1)){
        P1OUT ^= BIT0;              // Toggles the state of P1.0
    }
    TA0CTL = MC_0;              // Halts the timer
}
