/*
 * hal_timerA0.c
 *
 *  Created on: 20.01.2017
 *      Author: Sebo
 */

#include <msp430.h>
#include "hal_timerA1.h"
#include "hal_usc.h"

void HAL_TimerA0_Init()
{
	//////////////////////ZÄHLT MIT 62,5kHz///
	TA0CTL |= TASSEL__SMCLK+ID_3+MC__STOP;/*+CNTL__16*/    	//SEL_SMCLKK=Clock ist SMCLOCK    MC_STOP=Mode ist Stop
	TA0EX0 = TAIDEX_7;
	//TA1CCTL0 |= CM_0+CCIS_0+SCS+OUTMOD_0+CCIE;		//CM=Capture mode:No Capture     CCIS=Capt/compare Inpt select ist   SCS=Syncron Capture  OUTMOD_0=Out BIT-Value   CCIE=Interrupt enable
	//TA1CCTL1 |= CM_0+CCIS_0+SCS+OUTMOD_7;      //Throttle 7  Outmod: Reset/Set  für PWM
	TA0CCTL2 |= CM_2+CCIS_0+CAP+CCIE;

	TA0R = 0x0;   //Init Register Zähler
	//TA0CCR0 = Fr_divider_A; //0x3D09;  //-1 weil der 0te wert dazugehört   Divider siehe h-file
	//TA1CCR1= 2500;  //CC Regeister Throttle
	//TA1CCR2= 3590;  //CC Register Steering

	TA0CTL |=TACLR;//clear
	TA0CTL |= MC__CONTINUOUS;  //Mode Hochzählen
}
