/*
 * potentiometer.c
 *
 *  Created on: Oct 26, 2020
 *      Author: Aidan
 */
#include "msp.h"
#include "potentiometer.h"

#define BIT21                                     (uint32_t)(0x00100000)
#define BIT18                                     (uint32_t)(0x00020000)
#define BIT30                                     (uint32_t)(0x40000000)
#define BIT22                                     (uint32_t)(0x00200000)
#define BIT23                                     (uint32_t)(0x00400000)

int output;
float output1;
float p_distance;


    void ADC_config1(void){
        ADC14->CTL0 =0;
        ADC14->CTL0 &= ~BIT1;             //disables adc so it can be configured
        ADC14->CTL0 |= BIT30;             //predivides by 4
        ADC14->CTL0 |= BIT23;             //divides again by 4
        ADC14->CTL0 |= BIT22;             //helps with above
        ADC14->CTL0 |= BIT21;             // sets source to smclk
        ADC14->CTL0 |= BIT18;             // sets mode to repeat single channel
        ADC14->CTL0 |= BITC;             // sets hold time to 32 cycles
        ADC14->CTL0 |= BITD;             // helps with above
        ADC14->CTL0 |= BIT7;             // sets to repeat the conversion


        ADC14->CTL1 =0;                   // clrs previous settings for ctl1 also sets data to be converted to be stored in memory 0
                                          // also sets resolution to be 8 bit,   also sets to binary unsigned
       // ADC14->CTL1 |=BIT1;               // low power mode sampling up to 200 ksps

        ADC14->MCTL[0] = 0;
        ADC14->MCTL[0] &= ~BITD;          // sets to single ended mode
        ADC14->MCTL[0] |= BITB;           // next 4 lines set reference voltages to be 5.7 for low and 5.6 for high (need 5.7 connected to ground)
        ADC14->MCTL[0] |= BITA;
        ADC14->MCTL[0] |= BIT9;
        ADC14->MCTL[0] &= ~BIT8;

        ADC14->MCTL[0] &= ~BIT4;           //next 5 lines set input to be A14, P6.1
        ADC14->MCTL[0] |= BIT3;
        ADC14->MCTL[0] |= BIT2;
        ADC14->MCTL[0] |= BIT1;
        ADC14->MCTL[0] &= ~BIT0;

        ADC14->CLRIFGR0 |= BIT0;          // clrs interrupt flag of memory register

        ADC14->CTL0 |= BIT1;              // sets adc back to ready to convert
        ADC14->CTL0 |=BIT4;               // turns ADC back to on
}
    void ADC_config2(void){
        ADC14->CTL0 = 0;
            // sets adc on, uses smclk, single sample mode, 32  cycles for hold time, predidvides clk by 16
        ADC14->CTL0 |= ADC14_CTL0_ON | ADC14_CTL0_SHT0_3   | ADC14_CTL0_SSEL_4 |ADC14_CTL0_DIV_3 |ADC14_CTL0_SHP |ADC14_CTL0_PDIV_1;
            // sets power mode regular, unsigned binary, resolution 8 bits, memory 0,
        ADC14->CTL1 =0;
        ADC14->MCTL[0]=0;
            // sets input from A14, sets references from vref+ and vref-
        ADC14->MCTL[0] |= ADC14_MCTLN_INCH_15 |ADC14_MCTLN_VRSEL_14 ;
            // enables interrupts for memory 0
        ADC14->IER0 |=ADC14_IER0_IE0 ;

            //sets adc ready to convert
        ADC14->CTL0 |=ADC14_CTL0_ENC;
    }
    void GPIO_ADC_config1(void){
        P5->DIR &= ~BIT7;                         //sets the direction register to input
        P5->SEL0 &= ~BIT7;                        //sets p5.7 to its third function  Vref- (gnd)
        P5->SEL1 |= (BIT7);

        P5->DIR &= ~BIT6;                         //sets the direction register to input
        P5->SEL0 &= ~BIT6;                        //sets p5.6 to its third function  vref+ (3.3V)
        P5->SEL1 |= (BIT6);

        //P6->DIR &= ~BIT1;                         //sets the direction register to input
        P6->OUT  &= ~BIT0;                          // sets a pulldown resistor. not sure if this is needed.
        P6->REN  |= BIT0;
        P6->SEL0 |= BIT0;                         //sets p6.1 to be an analog pin
        P6->SEL1 &= ~BIT0;

        }

    void ADC_NVIC_setup(void){
        __NVIC_EnableIRQ(ADC14_IRQn);
    }

    void ADC14_IRQHandler(void){

        ADC14->CLRIFGR0 |=ADC14_CLRIFGR0_CLRIFG0;        //clrs the interrupt flag
        output =0;
        output = ADC14->MEM[0];                          //loads conversion result into variable
        output1= output;                                 //converts result to a float
        p_distance = output1/64 ;                        //converts result to a distance 0-4 Meters
        printf("p_distance is %f " ,p_distance);         //debug prints
        printf("output is %d \n ",output);

    }
