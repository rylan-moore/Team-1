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
//    TIMER_A0->CCTL[0]= TIMER_A_CCTLN_CCIE;   //Enable the interrupt for CCR0
//
//    TIMER_A0->CTL |= TIMER_A_CTL_TASSEL_2 ;              // Use SMCLK
//    TIMER_A0->CTL |= TIMER_A_CTL_CLR;                    // Clear the timer
//    TIMER_A0->CTL |= TIMER_A_CTL_ID__1;                  // Clock divider 1
//
//    TIMER_A0->CCR[0] = 3000;                   // 1ms at 3mhz Want to set to 3000 without a clock divider
//    TIMER_A0->CTL  |=  TIMER_A_CTL_MC__UP;     // Start the timer in Up mode
//
//    __NVIC_EnableIRQ(TA0_0_IRQn);              // Enable the TimerA interrupt in the nvic.
    /*              TIMER A0            */
    TIMER_A1->CCTL[0]= TIMER_A_CCTLN_CCIE;       // CCR0 interrupt enabled
    TIMER_A1->CCR[0] = 1000-1;                   // 1ms at 1mhz
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC__UP | TIMER_A_CTL_CLR;                  // SMCLK, upmode


    __NVIC_EnableIRQ(TA1_0_IRQn);
}
void TA1_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}



