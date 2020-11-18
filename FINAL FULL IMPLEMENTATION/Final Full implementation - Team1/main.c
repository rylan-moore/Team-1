
#include "msp.h"
#include <stdint.h>
#include "project.h"

int miliseconds = 0;
int distance = 0;
long sensor = 0;
int p_distance;
int state;

static void Delay(uint32_t loop)
{
    volatile uint32_t i;
    for (i = 0 ; i <  loop ; i++);
}

int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    GPIO_ADC_config1();
    ADC_config2();
    ADC_NVIC_setup();

    ADC14->MEM[0]=0;
    WDT_A->CTL = WDT_A_CTL_PW |             // Stop watchdog timer
            WDT_A_CTL_HOLD;

config_UART();
config_GPIO();
config_TA0();
gpio_interrput();
__enable_irq();             // Enables interrupts to the system
state = 1;

    while(state == 1){ //State machine
        ADC_run();
        US_trigger();
        US_echo();
        char buffer[50];
        sprintf(buffer,"distance is %d\n",distance);
        uart_out(buffer);
        Delay(100000);
        if(distance < p_distance && distance != 0){
            P1->OUT |= BIT0;  //turning LED on if distance is less than 20cm and if distance isn't 0.
            state = 2;

        }
        else{
            P1->OUT &= ~BIT0;
        }
    }
    if(state == 2){
        char buff3[50];
        sprintf(buff3,"End of program\n");
        uart_out(buff3);
    }
//    while(state == 2){
//        Delay(100000);
//        ADC_run();
//        if (distance > p_distance){
//            P1->OUT |= BIT0;
//            state = 1;
//        }
//    }

}


// ISR's
void PORT5_IRQHandler(void)
{
    if(P5->IFG & BIT2)                          //is there interrupt pending on 5.2?
    {
        if(!(P5->IES & BIT2))                   // is this the rising edge of the Echo
        {
            TIMER_A0->CTL |= TIMER_A_CTL_CLR;   // clears timer A
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

void TA0_0_IRQHandler(void)
{
    //    Interrupt gets triggered for every clock cycle in SMCLK Mode counting number of pulses
    miliseconds++;
    TIMER_A0->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}
void ADC14_IRQHandler(void){

    ADC14->CLRIFGR0 |=ADC14_CLRIFGR0_CLRIFG0;        //clears the interrupt flag
    p_distance = ADC14->MEM[0];                       //loads conversion result into variable
    char buff2[50];
    sprintf(buff2,"p_distance is %d " ,p_distance);         //debug prints
    uart_out(buff2);

}

//US functions
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
