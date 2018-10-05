#include <msp430.h> 

int main(void){
    WDTCTL = WDTPW + WDTHOLD;   // Stop WDT

    P1DIR |= BIT7;              // Sets P1.7 in the input direction
    P1SEL0 &= ~BIT7;            // Connects P1.7 to the timer
    P1SEL1 |= BIT7;             // Connects P1.7 to the timer
    P1OUT &= ~BIT7;

    TB0CTL = TBSSEL_2 +ID_2 + MC_1 +TBCLR;
                                // SMCLK, up mode
    TB0CCR0 = 250;              // Sets the PWM Period
    TB0CCR2 = 25;               // CCR2 PWM duty cycle
    TB0CCTL2 = OUTMOD_7;        // CCR2 reset/set

    //BUTTON
    P1DIR &= ~BIT1;             // Sets P1.1 in the input direction
    P1SEL0 &= ~BIT1;            // Connects P1.1 to I/O
    P1SEL1 &= ~BIT1;            // Connects P1.1 to I/O
    P1REN |= BIT1;              // P1.1 pullup/pulldown resistor enabled
    P1OUT |= BIT1;              // P1.1 pullup/pulldown configured as
                                // pull up resistor

    P1IE |= BIT1;               // P1.1 interrupt enabled
    P1IES |= BIT1;              // P1.1 interrupt flag is set with a high
                                // to low transition
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared

    _BIS_SR(LPM0_bits + GIE);   // Sets the processor to low processor mode
                                // and enables global interrupts
}

#pragma vector = PORT1_VECTOR
__interrupt void button_interrupt(void){
    if (P1IES & BIT1) {
        if (TB0CCR2 <= 250){
            TB0CCR2 += 25;
        } else{
            TB0CCR2 = 0;
        }
    }
    P1IFG &= ~BIT1;             // P1.1 interrupt flag is cleared
}

