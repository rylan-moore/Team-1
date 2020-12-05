
/* ECEN 2440 - F2020 - Team 1
 *
 * Team members:
 * Rylan Moore
 * Alexis Palau
 * Aidan Rogers
 * Abiel Tewolde
 *
 * PINS USED: ~~~~~~~~~~~~~~~~~~~~~
 * US - ECHO -> P5.2
 * US - TRIG -> P5.0
 * Potentiometer -> P6.0
 * ADC HR -> 5.6 (3v3)
 * ADC LR -> 5.7 (GND)
 *
 * UART:
 * Serial used for testing
 *
 * Pin 3.2 -> RX //USED FOR TESTING UART OUTPUT OVER GPIO
 * Pin 3.3 -> TX
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Sources: ~~~~~~~~~~~~~~~~~~~~~~~
 * TI Resource explorer helped us with UART driver
 *
 * Guide for using interrupts to get US data:
 * github.com/ChandraKiranSaladi/MSP432-Ultraosnic-UART/blob/master/SMP432-Ultrasonic.c
 *
 *
 * What is included:
 * -Ultrasonic data collection over GPIO pins, using Timer A
 * -Potentiometer data collection using ADC14
 * -UART over Serial for debugging data collection and state machine
 * -UART over GPIO pins for sending state change to wifi module (could be implemented in the future)
 *
 * Not included:
 * -Quade movement (hard to test and setup without quade itself)
 * -Wifi module (Basic communication line setup but module not tested or functioning)
 * -Other sensors used with quade: Again hard to implement without the board and parts for quade.
 * */

#include "msp.h"
#include <stdint.h>
#include "project.h"

int miliseconds = 0; //used for timing US sensor and calculating US distance
int distance = 0; //US distance
long sensor = 0; //RAW US sensor data calculation before unit scale applied
int p_distance; //potentiometer distance
int state ; //State variable

static void Delay(uint32_t loop) //Used as basic time delay
{
    volatile uint32_t i;
    for (i = 0 ; i <  loop ; i++);
}

int main(void)

{
    state = 0;
    while(state == 0){ //config state
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    GPIO_ADC_config1();
    ADC_config2();
    ADC_NVIC_setup();

    ADC14->MEM[0]=0;
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

    config_UART();
    config_UARTW(); //Testing for config wifi module NOTE THE WIFI MODULE DID NOT MAKE IT TO FINAL IMPLEMENTATION (UART OVER THESE PINS IS TESTED)
    config_GPIO();
    config_TA1();
    gpio_interrput();
    __enable_irq();             // Enables interrupts to the system
    state = 1;
    }
while(1){
    // Loop between the two states during operation.
    while(state == 1){ //moving forward state. In this section we are reading data from the US sensor and comparing it to the potentiometer input.
        //This state will send data over the serial port for debugging and when it detects an object close a message will be sent using UART over p3.3
        /*
         * This state runs both the potentiometer and US sensors to collect data on Quade's location relative to an object in its' path
         *  as well as what the user desires for the distance quade will stop from the object
         *
         * */
        ADC_run();
        US_trigger();
        US_echo();
        char buffer[50]; //UART buffer before calling the function to output over UART
        if (distance > 255){ //scales down a max reading to be within the range of values set on the adc.
            distance = 255;
        }
        sprintf(buffer,"distance is %d\n",distance);
        uart_out(buffer);

        Delay(100000);
        if(distance < p_distance && distance != 0){ //If quade is within the distance threshold set by the user
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
            state = 2;
            char buff2[50];
            sprintf(buff2,"object detected!\n"); //When the state changes due to an object close the msp will send a message over GPIO UART pins
            uart_outW(buff2); //Send data over p3.3
        }
        else{
            P1->OUT &= ~BIT0;
        }
    }
    while(state == 2){ //Stopped state. Will check the ADC until the value is less than the last value read by the US sensor
        /*This state would also have quade talk to the wifi module and send out a signal that quade is too close to an object.
         * */
        char buff3[50];
        sprintf(buff3,"object close!\n"); //send of serial monitor to debug this state
        uart_out(buff3);
        Delay(100000);
        ADC_run();
        if (distance > p_distance){ //Move back to state 1 if the threshold for stopping movement is lowered below the last US reading
            P1->OUT &= ~BIT0;
            state = 1;
        }
    }
}
}
