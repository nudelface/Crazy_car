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
//Verschiebt 4 Words direkt in die Struktur....Die ganze zeit
// Von Adresse der ADC-Mem register


void HAL_DMA_Init(void)
{
	DMACTL0|=DMA0TSEL_24;  //Dma trigger 24  = ADC12 IFG
	DMACTL4|=DMARMWDIS;   //Read-modify-write disable  W�hrend Prozesssor schreibt, stoppt DMA
	DMA0CTL&=~DMAIFG;    //No Interrupt pending reinschreiben f�r keine ahnung was
	DMA0CTL|=DMADT_1+DMAEN+DMASRCINCR_3+DMADSTINCR_3+DMAIE;  //Word-�bertragung  //DMADT1=Block Transfer  DMAEN=DMA Enable  DMASRCINCR3=Source adresse wird inkrementiert DMA Destination Adresse wird inkrementiert  DMAIE=Interrupt enabled
	DMA0SZ= 4;  //4Bytes werden transferriert

	__data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &ADC12MEM0);

	__data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &ADC1.Bit_right);


}
