/*
 * project.h
 *
 *  Created on: Nov 2, 2020
 *      Author: rylan
 */

#ifndef PROJECT_H_
#define PROJECT_H_

void config_TA0();
void US_trigger();
void US_echo();
static void Delay(uint32_t loop);
void config_UART();
int uart_puts(const char *str);
void config_GPIO();



#endif /* PROJECT_H_ */
