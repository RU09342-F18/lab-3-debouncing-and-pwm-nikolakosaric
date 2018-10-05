#include <msp430.h> 

int main(void){
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT
    P2DIR |= BIT1;              // Sets P2.1 in the input direction
    P2SEL |= BIT1;              // Connects P2.1 to the timer
    P2SEL2 &= ~BIT1;            // Connects P2.1 to the timer
    TA1CCR0 = 512-1;            // Sets the PWM Period
    TA1CCTL1 = OUTMOD_7;        // CCR1 reset/set
    TA1CCR1 = 0;                // CCR1 PWM duty cycle
    TA1CTL = TASSEL_2 + MC_1;   // SMCLK, up mode

    //BUTTON
    P1DIR &= ~BIT3;             // Sets P1.3 in the input direction
    P1REN |= BIT3;              // P1.3 pullup/pulldown resistor enabled
    P1OUT |= BIT3;              // P1.3 pullup/pulldown configured as
                                // pull up resistor

    P1IE |= BIT3;               // P1.3 interrupt enabled
    P1IES |= BIT3;              // P1.3 interrupt flag is set with a high
                                // to low transition
    P1IFG &= ~BIT3;             // P1.3 interrupt flag is cleared

    _BIS_SR(LPM0_bits + GIE);   // Sets the processor to low processor mode
                                // and enables global interrupts
}
#pragma vector = PORT1_VECTOR
__interrupt void button_interrupt(void){
    if (TA1CCR1 <= 510){        // If duty cycle is not 100%
        TA1CCR1 += 51;          // Increase duty cycle by 10%
    } else{                     // If not
        TA1CCR1 = 0;            // Sets duty cycle to 0%
    }
    P1IFG &= ~BIT3;             // P1.3 interrupt flag is cleared
}
