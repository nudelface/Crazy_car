/*
 * driver_aktorik.c
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */
#include <msp430.h>
#include "driver_aktorik.h"
#include "..\HAL\hal_timerA1.h"

extern int initcounter;



void Driver_SetSteering (int SteeringAngle)
{
	if (SteeringAngle<-560)
	{
		TA1CCR2=-560;
	}
	else if (SteeringAngle>560)
	{
		TA1CCR2=560;
	}
	else
	{
		TA1CCR2=3590+SteeringAngle;
	}
}

void Driver_ESCInit(void)
{
///  3 Perioden Random PWM
/// 140 PErioden Maximal R�ckw�rts also 1ms pwm   140 Perioden
/// 140 Perioden Minimal R�ckw�rts also 1.2ms
//  140 Perioden Maximal Brake 1.4ms
//  140 Perioden mimnimal Gas 1.6ms
/// 140 Perioden Maximal Gas  2ms


		Driver_PWMInit(2500,3);// in PWM Modus
		// __delay_cycles(5000);
		Driver_PWMInit(2500,140);// MAX rpw
		//__delay_cycles(5000);
		Driver_PWMInit(3000,140);// Min Rpw
		// __delay_cycles(5000);
		Driver_PWMInit(4000,140);// Min FPW
		//__delay_cycles(5000);
		Driver_PWMInit(5000,140);// Max FPW
		//__delay_cycles(5000);
		Driver_PWMInit(3500,1);// Max Brake


}


void Driver_SetThrottle (int Throttle)  //Gasgeben
{

}

void Driver_SetBrake (int Brake)
{}

void Driver_SetBack(int Backwards)
{}

void Driver_PWMInit(int PWM, int cycle)
{
	initcounter=0;
	while(initcounter<=cycle)
	{
		TA1CCR1=PWM;
	}
}
