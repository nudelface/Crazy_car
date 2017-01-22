/*
 * hal_usc.c
 *
 *  Created on: 25. Okt. 2016
 *      Author: Niki
 */
#include "hal_usc.h"


void HAL_UCS_Init(void)
{
		//P3DIR |= BIT4;                            // SMCLK set out to pins
	    //P3SEL |= BIT4;                         	//IO funktion or peripheral function

	    //P7SEL |= BIT2+BIT3;                       // Port select XT2

	    UCSCTL6 &= ~XT2OFF;                         // Enable XT2

	    UCSCTL3 |= SELREF_2;                        // FLLref = REFO
	                                                // Since LFXT1 is not used,
	                                                // sourcing FLL with LFXT1 can cause
	                                                // XT1OFFG flag to set
	    UCSCTL4 |= SELA_2;                          // ACLK=REFO,SMCLK=DCO,MCLK=DCO

	    // Loop until XT1,XT2 & DCO stabilizes - in this case loop until XT2 settles
	    do
	    {
	    UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
	                                            // Clear XT2,XT1,DCO fault flags
	    SFRIFG1 &= ~OFIFG;                      // Clear fault flags
	    }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag

	    UCSCTL6 &= ~XT2DRIVE1;

	    UCSCTL4 |= (SELS_5|SELM_5);                        // SMCLK=XT2CLK,MCLK=XT2CLK
	    UCSCTL5 |= DIVS__8;                     			//divide SMCLK by 8 to 2.5 MHz


}

