/*
 * hal_adc12.c
 *
 *  Created on: 29.11.2016
 *      Author: Sebo
 */


#include "hal_adc12.h"
#include <msp430.h>

ADC12Com ADC1;

void HAL_Adc_init(void)
{
	ADC12IE |= ADC12IE4;
	ADC12CTL2 |= ADC12PDIV+ADC12RES_2;
	ADC12CTL1 |= ADC12SHP+ADC12DIV_7+ADC12SHS_2+ADC12CONSEQ_1;    //repeat sequence channels, sampleholdsource ist timerB
	ADC12CTL0 |= ADC12MSC+ADC12REF2_5V+ADC12REFON+ADC12ON+ADC12ENC+ADC12SHT0_11;
	ADC12MCTL0 |= 0x0;
	ADC12MCTL1 |= 0x1;
	ADC12MCTL2 |= 0x2;
	ADC12MCTL3 |= 0x3;

	// Aktuelle AUflösung ist: 0,000913V / Bit
}

