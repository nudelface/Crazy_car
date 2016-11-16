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
int state=1;


void Driver_SetSteering (int SteeringAngle)  //Void funktion. Eingabe: -100% bis +100% Links ist minus Rechts ist plus
{
	static int StPWM = 0;
							// Mittelpunktkalibrrierung ist wichtig!!!!
	if ((SteeringAngle < 0) && (SteeringAngle >= -100) ) //Lenkunk nach link und kleiner als 100%, 100% entspricht dem linken Anschlagspunkt, �ber 100% kann die LEnkung zerst�ren
	{
		StPWM = StPWM_middle + (SteeringAngle * res_left); // PWM-Wrt wird berechnet. PWM=PWM_Mitte - X% * (PWM_mitte-Anschlag_links)/100
	}
	else if ((SteeringAngle>0) && (SteeringAngle<=100))
	{
		StPWM = StPWM_middle + (SteeringAngle * res_right); //PWM=PWM_Mitte + X% * (Anschlag_Rechts-PWM_mitte)/100
	}
	else if (SteeringAngle>100)
	{
		StPWM = StPWM_full_right;
	}
	else if (SteeringAngle<-100)
	{
		StPWM = StPWM_full_left;
	}
	else
	{
		StPWM = StPWM_middle;
	}

	if((StPWM>=StPWM_full_left) && (StPWM<=StPWM_full_right))
	{
		TA1CCR2 = StPWM;
	}  //Beschreiben der Compare-Registers  PWM setzen
	else
	{
		StPWM = StPWM_middle;
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

		Driver_PWMInit(MaxRPW,140);// MAX rpw

		Driver_PWMInit(MinRPW,140);// Min Rpw

		Driver_PWMInit(MinFPW,140);// Min FPW

		Driver_PWMInit(MaxFPW,140);// Max FPW

		Driver_PWMInit(MaxBRK,140);// Max Brake


}


void Driver_SetThrottle (int Throttle)  		//Gasgeben
{

	static int ThPWM=0;
	state=1;

	if((Throttle >= 0) && (Throttle <= 100))  	//Gasbereich begrenzen
	{
		ThPWM = MinFPW + res_throttle * Throttle; 	 // Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
	}
	if((ThPWM>=MinFPW) && (ThPWM<=MaxFPW))  		//Vorsichtsma�nahme, damit wirklich keine falsche PWM gesetzt wird
	{
		TA1CCR1 = ThPWM;
	}

}

void Driver_SetBrake (int Brake)    //Bremsen
{

	static int BrPWM = 0;
	if((Brake >= 0) && (Brake <= 100))  	//Gasbereich begrenzen
	{
		if (state == 1)
		{
			BrPWM = MinFPW - res_brk_f * Brake;
		}// Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
		else if(state==2)
		{
			BrPWM = MinRPW + res_brk_f * Brake;
		}
	}
	TA1CCR1 = BrPWM;
}

void Driver_SetBack(int Backwards)
{
	state = 2;
	static int BackPWM = 0;

	if((Backwards>=0) && (Backwards<=100))  	//Gasbereich begrenzen
	{
		BackPWM = MinFPW - res_back * Backwards; 	 // Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
	}
	if((BackPWM>=MaxRPW) && (BackPWM<=MinRPW))  		//Vorsichtsma�nahme, damit wirklich keine falsche PWM gesetzt wird
	{
		TA1CCR1 = BackPWM;
	}
}

void Driver_PWMInit(int PWM, int cycle)
{
	initcounter = 0;
	while(initcounter <= cycle)
	{
		TA1CCR1 = PWM;
	}
}