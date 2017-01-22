/*
 * hal_timerA1.c
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */

#include <msp430.h>
#include "hal_timerA1.h"
#include "hal_usc.h"




void HAL_TimerA1_Init()
{
	TA1CTL |= TBSSEL__SMCLK+ID_0+MC__STOP;/*+CNTL__16*/    	//SEL_SMCLKK=Clock ist SMCLOCK    MC_STOP=Mode ist Stop
	TA1CCTL0 |= CM_0+CCIS_0+SCS+OUTMOD_0+CCIE;		//CM=Capture mode:No Capture     CCIS=Capt/compare Inpt select ist   SCS=Syncron Capture  OUTMOD_0=Out BIT-Value   CCIE=Interrupt enable
	TA1CCTL1 |= CM_0+CCIS_0+SCS+OUTMOD_7;      //Throttle 7  Outmod: Reset/Set  für PWM
	TA1CCTL2 |= CM_0+CCIS_0+SCS+OUTMOD_7;      //Steering   Outmod: Reset/Set  für PWM

	TA1R = 0x0;   //Init Register Zähler
	TA1CCR0 = Fr_divider_A; //0x3D09;  //-1 weil der 0te wert dazugehört   Divider siehe h-file
	TA1CCR1= 2500;  //CC Regeister Throttle
	TA1CCR2= 3590;  //CC Register Steering

	TA1CTL |=TACLR;//clear
	TA1CTL |= MC__UP;  //Mode Hochzählen

}
