
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
