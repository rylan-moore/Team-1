#include "msp.h"
#include "project.h"
#include "stdio.h"


int miliseconds = 0;
int distance = 0;
long sensor = 0;

/**
 * main.c
 */
int main(void)
{
    uint32_t i;
    uint32_t error = 0;

    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

//config gpio
config_GPIO();
    //config uart
    config_UART();
        //config timerA0
        config_TA1();

    __enable_irq();             // Enables interrupts to the system

    NVIC->ISER[1] = 1 << ((PORT5_IRQn) & 31);       // Very important to assign interrupts to the NVIC vector otherwise they would not
                                                    // considered
    NVIC->ISER[0] = 1 << ((TA1_0_IRQn) & 31);
    //    __low_power_mode_3();
    //    __no_operation();

    while(1){

//trigger
        US_trigger();

//echo
        US_echo();

        char buffer[50];
        sprintf(buffer,"distance is %d\n",distance);
        uart_puts(buffer);
        Delay(100000);
        if(distance < 20 && distance != 0)
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
        else
            P1->OUT &= ~BIT0;

    }

}
//** IRQ's


void PORT5_IRQHandler(void)
{

    if(P5->IFG & BIT2)  //is there interrupt pending?
    {
        if(!(P5->IES & BIT2)) // is this the rising edge?
        {

            TIMER_A1->CTL |= TIMER_A_CTL_CLR;   // clears timer A
            miliseconds = 0;
            P5->IES |=  BIT2;  //falling edge
        }
        else
        {
            sensor = (long)miliseconds*100;// - (long)TIMER_A0->R;    //calculating ECHO length
            //            P1->OUT ^= BIT0;
            P5->IES &=  ~BIT2;  //falling edge

        }
        P5->IFG &= ~BIT2;             //clear flag
    }
}

void TA1_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    //    printf("milliseconds: "+miliseconds);
}

