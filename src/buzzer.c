#include <buzzer.h>

// Initialize the buzzer on MKII and its PWM control
void Buzzer_init(void)
{
    // Set P2.7 (J4.40) to be a GPIO Output Pin
    P2->DIR |= BIT7;

    // Select TimerA PWM controller
    // P2.7 <--> TA0.4
    P2->SEL0 |= BIT7;
    P2->SEL1 &= ~BIT7;

    // Turn off TA0
    TIMER_A0->CTL = 0;

    // Configure TA0.4 for Reset/set Mode
    TIMER_A0->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7;

    // Select the master clock as the timer source
    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK;

    // Clear the current Mode Control Bits (Stop mode)
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}

// Set the PWM period and turn on the buzzer on MKII
// Parameters: Period of PWM Pulse
void turn_Buzzer_On(uint16_t ticks_period)
{
    // Stop the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;

    // set the period of the timer
    TIMER_A0->CCR[0] = ticks_period - 1;

    // configure the duty cycle of the buzzer to 50%
    TIMER_A0->CCR[4] = ticks_period / 2 - 1;

    // clear the current count and set the timer to Up mode
    TIMER_A0->CTL |= TIMER_A_CTL_CLR | TIMER_A_CTL_MC__UP;
}


// Turn off the buzzer on MKII
void turn_Buzzer_Off(void)
{
    // Turn off the timer
    TIMER_A0->CTL &= ~TIMER_A_CTL_MC_MASK;
}


// Busy waits for input clock period and return
// Parameters: number of cycle to wait
void T32_1_wait_period(uint32_t period)
{
    // stop the timer
    TIMER32_1->CONTROL = 0;
    // set the timer to be one shot
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ONESHOT;
    // set pre-scale, clock divided by 256
    TIMER32_1->CONTROL |= TIMER32_CONTROL_PRESCALE_2;
    // set the load register
    TIMER32_1->LOAD = period / 256;
    // start the timer
    TIMER32_1->CONTROL |= TIMER32_CONTROL_ENABLE;
    // wait until it reaches 0
    while (TIMER32_1->VALUE != 0)
    {
        ;
    }
}
