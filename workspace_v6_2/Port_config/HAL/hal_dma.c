/*
 * hal_dma.c
 *
 *  Created on: 13.12.2016
 *      Author: Sebo
 */
#include "hal_dma.h"
#include "hal_adc12.h"
#include <msp430.h>

extern ADC12Com ADC1;

void HAL_DMA_Init(void)
{
	DMACTL0|=DMA0TSEL_24;
	DMACTL4|=DMARMWDIS;
	DMA0CTL |= DMADT_5+DMASRCINCR_3+DMADSTINCR_3;
	DMA0CTL&=~DMAIFG;
	DMA0CTL|=DMAEN+DMAIE;
	DMA0SZ|= 0x4;
	//DMA0DA|= (unsigned long)&ADC1.Bit_right;
	__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &ADC12MEM0);
	__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &ADC1.Bit_right);

	//DMA0SA|=(unsigned long) &ADC12MEM0;



}