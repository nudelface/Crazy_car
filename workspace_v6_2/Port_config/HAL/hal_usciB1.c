/*
 * hal_usciB1.c
 *
 *  Created on: 07.11.2016
 *      Author: Sebo
 */

#include <msp430.h>
#include "hal_usciB1.h"
#include "HAL\hal_gpio.h"

extern USCIB1_SPICom SpiCom;



void HAL_USCIB1_Init(void)
{
	UCB1CTL1|=UCSWRST;
	UCB1CTL0|=UCCKPL+UCMSB+UCMST+UCSYNC+UCMODE0;
	UCB1CTL1|=UCSSEL__SMCLK;
	UCB1STAT|=UCLISTEN;
	UCB1BR0=SCLK_diver;   // Divider = 25
	UCB1IE|=0x1;
	UCB1CTL1&=UCSSEL__SMCLK+UCSWRST*0;
	//UCB1STE
	UCB1IE|=0x1;

}


void HAL_USCIB1_Transmit(void)
{
	SpiCom.TxData.cnt=0;
	SpiCom.Status.B.TXSuc=0;
	UCB1TXBUF=SpiCom.TxData.Data[0];


}
