/*
 * hal_adc12.c
 *
 *  Created on: 28. Nov. 2016
 *      Author: Nikolaus Lacher
 */

#include "hal_adc12.h"


 void HAL_ADC12_Init()
 {
	 ADC12CTL0 |= ADC12SC + ADC12ENC + ADC12OVIE
 }
