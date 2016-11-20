/*
 * Interrupts.c
 *
 *  Created on: 17.10.2016
 *      Author: Sebastian
 */
#include <msp430.h>
#include "Interrupts.h"

#include "../Driver/driver_aktorik.h"
#include "hal_gpio.h"
#include "hal_general.h"
#include "..\Driver\driver_general.h"
#include "hal_usciB1.h"


ButtonCom Buttons;
int SteeringCalibC = 0;
int initcounter = 0;
int timebase = 0;
int pressed = 0;
int lock_B=0;
int lock_hold;
extern USCIB1_SPICom SpiCom;


#pragma vector=PORT1_VECTOR     //Interrupt Vektor Port 1  read Buttons
__interrupt void PORT1_ISR (void)
{
	Buttons.active=1;
	if (START_flag>=1)
	{
		Buttons.button = 1;
		lock_B = 1;
		timebase = 0;
	}
	else if (STOP_flag>=1)
	{
		Buttons.button = 2;
		lock_B = 1;
		timebase = 0;
	}
	P1IFG &= ~START_Button;
	P1IFG &= ~STOP_Button;
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMERB_ISR (void)
{
	if((lock_B==1) || (lock_hold==1))
	{
		timebase += 1;

		if ((lock_B==1) && (timebase>=50) && (lock_hold==0))
		{
			P1IFG &= ~START_Button;
			P1IFG &= ~STOP_Button;
			pressed += 1;
		}
		else if ((timebase>200) && (lock_hold==1))
		{
			P1IFG &= ~START_Button;
			P1IFG &= ~STOP_Button;
		}

	}

	TBCTL |= TBCLR;
	TBEX0 |= TBIDEX__2;										// divide/2
	TBCTL |= MC__UP;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMERA_ISR (void)
{
	if(initcounter<=1000)
	{

	initcounter += 1;
	}
	if(SteeringCalibC<=100)
	{
		SteeringCalibC += 1;
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
		SpiCom.RXData.len=SpiCom.TxData.cnt;
	}
	else
	{
		SpiCom.Status.B.TXSuc=1;
		SpiCom.TxData.cnt=0;
	}
}




}
