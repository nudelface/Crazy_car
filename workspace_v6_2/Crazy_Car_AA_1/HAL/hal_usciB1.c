/*
 * hal_usciB1.c
 *
 *  Created on: 07.11.2016
 *      Author: Sebo
 */

#include <msp430.h>
#include "hal_usciB1.h"
#include "hal_gpio.h"

extern USCIB1_SPICom SpiCom;



void HAL_USCIB1_Init(void)
{
	UCB1CTL1|=UCSWRST;          //Enabled USCI Logic held in reset state
	UCB1CTL0|=UCCKPL+UCMSB+UCMST+UCSYNC+UCMODE0;  //UCCKPL=Clock Polarity:inactive state is high   UCMSB=MSB Firtst  UCMST=Master Mode   UCSYNC=Synchronous Mode enabled  UCMODE 0 = 3-Pin SPI
	UCB1CTL1|=UCSSEL__SMCLK;    //Clock select:  Source ist SMCLK
	UCB1STAT|=UCLISTEN;      // Loop-Back enable  Transmitted obejct werden Intern an den Receive-Register geschrieben
	UCB1BR0=SCLK_diver;   // Divider = 25
	UCB1IE|=0x1;              //Interrpt Enable
	UCB1CTL1 &= UCSSEL__SMCLK+UCSWRST*0;  //SMCLOCK selected und Wurst auf 0 gesetzt = SPI geht in Operation...also angeschalten
	UCB1IE|=0x1;

}


void HAL_USCIB1_Transmit(void)
{
	UCB1CTL1 &= ~UCSWRST;  //UsCI aktiv
	UCB1IE |= UCRXIE;     //Interrupt enable
	SpiCom.TxData.cnt=0;   //Init counter of Bytes
	SpiCom.Status.B.TXSuc=0;  //Setzen auf "in sendemodus"
	UCB1TXBUF=SpiCom.TxData.Data[0];  //Transmit starten
	while(SpiCom.Status.B.TXSuc==0); //Bleibt hier stehen bis datenversendet sind
}
