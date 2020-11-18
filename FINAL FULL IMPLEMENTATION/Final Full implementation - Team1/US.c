//#include "msp.h"
//#include "potentiometer.h"
///*
// * US.c
// *
// *  Created on: Nov 2, 2020
// *      Author: rylan
// */
// static void Delay(uint32_t loop) //May want to migrate into using a timer
//{
//    volatile uint32_t i;
//
//    for (i = 0 ; i < 48 * loop ; i++);
//}
//
//
//void US_trigger(){
//    P5->DIR |= BIT0;          // trigger pin as output
//    P5->OUT |= BIT0;          // generate pulse on the trigger pin
//    Delay(100);               // for 10us //This needs to be udpated a little more
//    P5->OUT &= ~BIT0;         // stop pulse
//}
//
//void US_echo(){
//    P5->IFG = 0;              // clear P2 interrupt
//    P5->IES &= ~BIT2;         // rising edge on ECHO pin
//    Delay(30000);             // delay for 30ms (after this time echo times out if there is no object detected) Allows the P5.2 interrupt to run
//    distance = sensor/58;     // converting ECHO length in cm
//}
//




