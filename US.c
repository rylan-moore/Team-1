#include "msp.h"
#include "project.h"
/*
 * US.c
 *
 *  Created on: Nov 2, 2020
 *      Author: rylan
 */

extern int distance;
extern int sensor;
extern int miliseconds;

 static void Delay(uint32_t loop) //May want to migrate into using a timer
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}


void US_trigger(){
    P5->DIR |= BIT0;          // trigger pin as output
    P5->OUT |= BIT0;          // generate pulse on the trigger pin
    Delay(100);               // for 10us //This needs to be udpated a little more
    P5->OUT &= ~BIT0;         // stop pulse
}

void US_echo(){
    P5->IFG = 0;              // clear P2 interrupt
    P5->IES &= ~BIT2;         // rising edge on ECHO pin
    Delay(30000);             // delay for 30ms (after this time echo times out if there is no object detected) Allows the P5.2 interrupt to run
    distance = sensor/58/2;     // converting ECHO length in cm
}
void PORT5_IRQHandler(void)
{
    if(P5->IFG & BIT2)                          //is there interrupt pending on 5.2?
    {
        if(!(P5->IES & BIT2))                   // is this the rising edge of the Echo
        {
            TIMER_A1->CTL |= TIMER_A_CTL_CLR;   // clears timer A
            miliseconds = 0;                    // start counting time of the pulse
            P5->IES |=  BIT2;                   // reset to falling edge
        }
        else
        {
            sensor = (long)miliseconds*100;     // If it is the falling edge the signal is done. calculate the sensor output
            P5->IES &=  ~BIT2;                  // Reset to not be the falling edge
        }
        P5->IFG &= ~BIT2;                       //clear flag
    }
}





