/*
 * potentiometer.h
 *
 *  Created on: Oct 26, 2020
 *      Author: Aidan
 */

#include "msp.h"

#ifndef POTENTIOMETER_H_
#define POTENTIOMETER_H_



int p_dist;
void ADC_config1(void);
void ADC_config2(void);
void GPIO_ADC_config1(void);
void ADC_NVIC_setup(void);


#endif /* POTENTIOMETER_H_ */
