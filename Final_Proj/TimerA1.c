#include "msp.h"
/*
 * TimerA1.c
 *
 *  Created on: Nov 2, 2020
 *      Author: rylan
 */
void config_TA1(){
        /*              TIMER A1            */
    TIMER_A1->CCTL[0]= TIMER_A_CCTLN_CCIE;       // CCR0 interrupt enabled
    TIMER_A1->CTL = TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_CLR |  TIMER_A_CTL_ID__1;                // SMCLK, upmode

    TIMER_A1->CCR[0] = 3000;                   // 1ms at 1mhz Want to set to 3000 without a clock divider
    TIMER_A1->CTL  |=  TIMER_A_CTL_MC__UP;
}
