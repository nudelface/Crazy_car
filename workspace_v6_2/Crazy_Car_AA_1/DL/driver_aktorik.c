/*
 * driver_aktorik.c
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */
#include <msp430.h>
#include "..\HAL\hal_timerA1.h"
#include "driver_aktorik.h"


extern int initcounter;    //Z�hlt bei PWM-Interrupt (Periode) hoch  Counts up when Pwm-Interrupt is set. (Every PWM-Period)



void Driver_SetSteering (int SteeringAngle)								  //Void funktion. Eingabe: -100% bis +100% Links ist minus Rechts ist plus
{
	int StPWM = 0;
																		// Mittelpunktkalibrrierung ist wichtig!!!!
	if ((SteeringAngle < 0) && (SteeringAngle >= -100) ) 				//Lenkunk nach link und kleiner als 100%, 100% entspricht dem linken Anschlagspunkt, �ber 100% kann die LEnkung zerst�ren
	{
		StPWM = StPWM_middle+(SteeringAngle * res_left); 				// PWM-Wrt wird berechnet. PWM=PWM_Mitte - X% * (PWM_mitte-Anschlag_links)/100
	}
	else if ((SteeringAngle > 0) && (SteeringAngle <= 100))
	{
		StPWM = StPWM_middle+ (SteeringAngle * res_right);				 //PWM=PWM_Mitte + X% * (Anschlag_Rechts-PWM_mitte)/100
	}
	else if (SteeringAngle > 100)
	{
		StPWM = StPWM_full_right;
	}
	else if (SteeringAngle < -100)
	{
		StPWM = StPWM_full_left;
	}
	else
	{
		StPWM = StPWM_middle;
	}

	if((StPWM >= StPWM_full_left) && (StPWM <= StPWM_full_right))
	{
		TA1CCR2=StPWM;
	}  //Beschreiben der Compare-Registers  PWM setzen
	else
	{
		StPWM=StPWM_middle;
	}


}

void Driver_ESCInit(void)
{
///  3 Perioden Random PWM
/// 140 PErioden Maximal R�ckw�rts also 1ms pwm   140 Perioden
/// 140 Perioden Minimal R�ckw�rts also 1.2ms

//  140 Perioden mimnimal Gas 1.6ms
/// 140 Perioden Maximal Gas  2ms
//  140 Perioden Maximal Brake 1.4ms

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
		Driver_PWMInit(3500,140);// Max Brake


}


void Driver_SetThrottle (int Throttle)  //Gasgeben
{
	if((Throttle >= 0)&&(Throttle <= 100))
	{

	}
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
