
#include "msp.h"
#include "project.h"
/*
 * GPIO.c
 *
 *  Created on: Nov 2, 2020
 *      Author: Team - 1
 */

#include <stdint.h>

void config_GPIO(){
    // Configure the LED for a test output
    P1->DIR |= BIT0;                        // Set P1.0 as output
    P1->OUT |= BIT0;                        // P1.0 high
    P1->OUT &= ~BIT0;


    //Configure pin 5.2 for the echo pin on the US sensor
    P5->DIR &= ~BIT2;            // Using pull down resistor
    P5->REN |= BIT2;             //
    P5->OUT &= ~BIT2;            //

    P5->SEL0 = 0; //Set everything to the standard function
    P5->SEL1 = 0;

    P5->IFG = 0;    //Reset the flag and enable the Interrupt on P5.2
    P5->IE |= BIT2;
    P5->IES &= ~BIT2;

    // Configure UART pins (Set to secondary function)
    P1->SEL0 |= BIT2;
    P1->SEL0 |= BIT3;

    P3->SEL0 |= BIT2; //for uartw
    P3->SEL0 |= BIT3;

}

void gpio_interrput(){
    __NVIC_EnableIRQ(PORT5_IRQn);
    //Enable interrupt on the pin used for the ultrasonic echo
}

