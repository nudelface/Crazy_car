/*
 * hal_timerB0.c
 *
 *  Created on: 25. Okt. 2016
 *      Author: Niki
 */

#include <msp430.h>
#include "hal_timerB0.h"
#include "hal_usc.h"


void HAL_TimerB0_Init()
{
	TBCTL |= 0x0+TBSSEL__SMCLK+CNTL__16+ID_3+MC__UP+TBIE;	//TBCLGRP=00 indipendent  CNTL = 00 16bit TBSSEL=10 SMCLK  ID=11
	TBCCTL0 |= CM_1+CCIS_0+SCS+CLLD_0+OUTMOD_4+CCIE;		//CM=Capture mode:1 CCIS=Capture inputCCIxA SCS=sychronize
															//CLLD=loads on write OUTMODE=toggle CCIE=interrupt
	TBR |= 0x0;
	TBCCR0 = Fr_divider_B; //0x3D09;  //-1 weil der 0te wert dazugehört
	TBEX0 |= TBIDEX__5;	// divide/5

	TBCTL |=TACLR;//clear
	TBCTL |= MC__UP;




}
