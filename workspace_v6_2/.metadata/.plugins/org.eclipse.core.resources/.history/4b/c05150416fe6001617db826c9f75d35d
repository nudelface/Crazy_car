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
	TBCTL |= 0x0+TBSSEL__SMCLK+CNTL__16+ID_3+MC__UP+TBIE;	//0x0 zum init   TBSource=SMCLOCK , CNTL_16=Counter Länge 16,  ID_3 = Divider durch 4 (input CLK)    , MC_Up = Mode hochzählen bis TB0CL0  ,  Interrupt Enabled
	TBCCTL0 |= CM_1+CCIS_0+SCS+CLLD_0+OUTMOD_4+CCIE;		//CM_1= Capture on rising Edge   CCIS_0=CCIxA CCR input oder so   SCS=Synchronized capture  CCLD_0=TBxCLn wird geladen wenn auf TBCCR geschrieben wird   Outmod_4=Toggle
															//CLLD=loads on write OUTMODE=toggle CCIE=interrupt
	TBR |= 0x0;  // Zähler Init
	TBCCR0 = Fr_divider_B1; //0x3D09;  //-1 weil der 0te wert dazugehört   Vergleichsregister
	TBEX0 |= TBIDEX__5;	// divide/5  //Predivieder von 5

	TBCTL |=TACLR;//clear
	TBCTL |= MC__UP;  //Starte Hochzählen




}
