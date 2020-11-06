#include "msp.h"
#include "potentiometer.h"

/**
 * main.c
 */
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    GPIO_ADC_config1();
    ADC_config2();
    ADC_NVIC_setup();

    ADC14->MEM[0]=0;
int i;

    while(1){


        for(i=20000; i>0; i--);
        ADC14->CTL0 |= ADC14_CTL0_SC  | ADC14_CTL0_ENC;
    }

}

