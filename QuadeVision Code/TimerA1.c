#include "msp.h"
#include "project.h"
/*
 * TimerA1.c
 *
 *  Created on: Nov 2, 2020
 *      Author: rylan
 */
extern int miliseconds;
void config_TA1(){
//        /*              TIMER A1            */
    TIMER_A1->CCTL[0]= TIMER_A_CCTLN_CCIE;       // CCR0 interrupt enabled
    TIMER_A1->CCR[0] = 1000;                     // SMCLK is used but is set to 1mhz instead of 3.
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR | TIMER_A_CTL_ID__1;                  // SMCLK, upmode


    __NVIC_EnableIRQ(TA1_0_IRQn);
}
void TA1_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}


