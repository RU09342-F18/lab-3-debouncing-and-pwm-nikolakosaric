#include <msp430.h> 

int main(void) {
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    //UCSCTL4 = SELA_0;           //

    //P1SEL &= ~(BIT0 + BIT3);    // Selects I/O function
    //P1SEL2 &= ~(BIT0 + BIT3);   // Selects I/O function

    PM5CTL0 &= ~LOCKLPM5;

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

    // Timer
    TB0CTL = TBSSEL_2 + ID_2 + MC_1 + TBCLR + TBIE;
                                // Sets TB clock to SMCLK, up mode, sets
                                // the internal divider to 4, and 4, clears
                                // the clock, and enables interrupts
    TB0CCR0 = 250;              // Sets the PWM period
    TB0CCR1 = 150;              // Sets the duty cycle
    TB0CCTL1 = CCIE;            //

    _BIS_SR(LPM0_bits + GIE);   // Sets the processor to low processor mode
                                // and enables global interrupts

    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void button_interrupt(void){
    if(P1IES & BIT1){           // Detects falling edge
        TB0CCR1 += 25;          // Increases duty cycle by %10

        if(TB0CCR1 > 250){      // If duty cycle is over 100%
            TB0CCR1 = 0;        // Sets duty cycle to 0
            P1OUT &= ~BIT0;     // Shuts off the LED
            TB0CTL = TBCLR;     // Clears the timer
        }else{
            TB0CTL = TBSSEL_2 + ID_2 + MC_1 + TBIE;
                                // Sets TA clock to SMCLK, up mode, sets
                                // the internal divider to 4, and 4, clears
                                // the clock, and enables interrupts
        }
    }

    P1IES ^= BIT1;              //Toggles edge select
    P1IFG &= ~BIT1;             //Clears interrupt flag
}

#pragma vector = TIMER0_B1_VECTOR;
__interrupt void Timer0B0 (void){
    switch(TB0IV){
        case 2:                 //
            P1OUT &= ~BIT0;     // Turning off the LED for button state
        break;
        case 14:                // Overflow detect
            P1OUT |= BIT0;
        break;
    }
}


