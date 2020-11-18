/*
 * potentiometer.c
 *
 *  Created on: Nov 5, 2020
 *      Author: Aidan
 */
#include "msp.h"
#include "project.h"


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
        P6->SEL0 |= BIT0;                         //sets p6.0 to be an analog pin
        P6->SEL1 &= ~BIT0;

        }

    void ADC_NVIC_setup(void){
        __NVIC_EnableIRQ(ADC14_IRQn);
    }
    void ADC_run(void){
        ADC14->CTL0 |= ADC14_CTL0_SC  | ADC14_CTL0_ENC;
    }






