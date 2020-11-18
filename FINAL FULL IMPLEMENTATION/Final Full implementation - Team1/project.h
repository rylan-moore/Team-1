/*
 * project.h
 *
 *  Created on: Nov 18, 2020
 *      Author: rylan
 */

#ifndef PROJECT_H_
#define PROJECT_H_


void US_trigger();
void US_echo();
void config_UART();
void gpio_interrput();
void ADC_run(void);
void config_GPIO();
void config_TA1();
void uart_out(const char *str);
void ADC_config1(void);
void ADC_config2(void);
void GPIO_ADC_config1(void);
void ADC_NVIC_setup(void);
int uart_puts(const char *str);




#endif /* PROJECT_H_ */
