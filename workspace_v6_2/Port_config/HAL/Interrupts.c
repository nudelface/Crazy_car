/*
 * Interrupts.c
 *
 *  Created on: 17.10.2016
 *      Author: Sebastian
 */
#include <msp430.h>
#include "Interrupts.h"
#include "hal_gpio.h"
#include "hal_general.h"
#include "..\Driver\driver_general.h"
#include "..\Driver\driver_aktorik.h"
#include "hal_usciB1.h"
#include "hal_adc12.h"
#include "..\Driver\driver_lcd.h"


ButtonCom Buttons;
int SteeringCalibC=0;
int initcounter=0;
extern USCIB1_SPICom SpiCom;
extern ADC12Com ADC1;
int counterz=0;

int state=0;

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR (void)
{
	Buttons.active=1;
	if (START_flag>=1)
			{Buttons.button=1;}
	else if (STOP_flag>=1)
			{Buttons.button=2;}



	P1IFG&=~START_Button;
	P1IFG&=~STOP_Button;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMERB_ISR (void)
{

	if (state==0)
	{
	LCD_BL_ON;
	state=1;
	}
	else
	{
		LCD_BL_OFF;
		state = 0;
	}
	counterz++;


	ADC1.Status.B.ADCrdy=0;




	TBCTL |= TBCLR;
	TBEX0 |= TBIDEX__5;										// divide/5
	TBCTL |= MC__UP;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMERA_ISR (void)
{
	if(initcounter<=1000)
	{

	initcounter+=1;
	}
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCB_ISR (void)
{

if((UCB1IFG&UCRXIFG)==1)
{
	SpiCom.RXData.Data[SpiCom.TxData.cnt]=UCB1RXBUF;

	SpiCom.TxData.cnt=SpiCom.TxData.cnt+1;

	if(SpiCom.TxData.cnt <= SpiCom.TxData.len-1)
	{

		UCB1TXBUF=SpiCom.TxData.Data[SpiCom.TxData.cnt];
		SpiCom.TxData.Data[SpiCom.TxData.cnt] = 0x0;
		SpiCom.RXData.len=SpiCom.TxData.cnt;
	}
	else
	{
		SpiCom.Status.B.TXSuc=1;
		SpiCom.TxData.cnt=0;
	}
}




}

/*
#pragma vector=ADC12_VECTOR
__interrupt void ADC_ISR (void)
{

	if(ADC12IV==0xC)
{
	ADC1.Bit_right=ADC12MEM0;
	ADC1.Bit_left=ADC12MEM1;
	ADC1.Bit_front=ADC12MEM2;
	ADC1.vBat=ADC12MEM3;
	ADC1.Status.B.ADCrdy=1;
	//ADC12IFG&=~ADC12IFG3;
	ADC12CTL0 |= ADC12ENC;
}
}
*/


#pragma vector=DMA_VECTOR
__interrupt void DMA_ISR(void)
{

	if(DMAIV==0x02)
	{
	ADC1.Status.B.ADCrdy=1;
	DMA0CTL|=DMAEN;
	ADC12CTL0 |= ADC12ENC;
	}
}

