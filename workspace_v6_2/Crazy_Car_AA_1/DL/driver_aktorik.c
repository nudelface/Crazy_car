/*
 *
 *
 *
 *
 *
 * driver_aktorik.c
 *
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */
#include <msp430.h>
#include "driver_aktorik.h"
#include "..\HAL\hal_timerA1.h"



extern int initcounter;
int state_akt=1;



///////////////////////////////////LENKUNG //////////////////////////////////

void Driver_SetSteering (int SteeringAngle)  //Void funktion. Eingabe: -100% bis +100% Links ist minus Rechts ist plus
{
	static int StPWM = 0;

	if ((SteeringAngle < 0) && (SteeringAngle >= -100) ) //Lenkung nach Links und kleiner als 100%, 100% entspricht dem linken Anschlagspunkt, über 100% kann die Lenkung zerstört werden
	{
		StPWM = StPWM_middle + (SteeringAngle * res_left); // PWM-Wert wird berechnet. PWM=PWM_Mitte - X% * (PWM_mitte-Anschlag_links)/100
	}
	else if ((SteeringAngle>0) && (SteeringAngle<=100)) // Lenkung Rechts
	{
		StPWM = StPWM_middle + (SteeringAngle * res_right); //PWM=PWM_Mitte + X% * (Anschlag_Rechts-PWM_mitte)/100
	}
	else if (SteeringAngle>100) //Anschlag Rechts
	{
		StPWM = StPWM_full_right;
	}
	else if (SteeringAngle<-100) // Anschlag Links
	{
		StPWM = StPWM_full_left;
	}
	else
	{
		StPWM = StPWM_middle; //Setze auf Mittelstellung
	}
	if((StPWM>=StPWM_full_left) && (StPWM<=StPWM_full_right))  //Anschlagprüfung 2
	{
		TA1CCR2 = StPWM;                                     //Schreibe PWM-Wert
	}
	else
	{
		StPWM = StPWM_middle;               //Ansonsten middle
	}

}



///////////////////////////////////  ESC Init    //////////////////////////////////

void Driver_ESCInit(void)
{


///  3 Perioden Random PWM
/// 140 Perioden Maximal Rückwärts
/// 140 Perioden Minimal Rückwärts
//  140 Perioden mimnimal Gas
/// 140 Perioden Maximal Gas
//  140 Perioden Maximal Brake

//	Siehe h-file

		Driver_PWMInit(MaxRPW,3);// in PWM Modus

		Driver_PWMInit(MaxRPW,140);// MAX rpw

		Driver_PWMInit(MinRPW,140);// Min Rpw

		Driver_PWMInit(MinFPW,140);// Min FPW

		Driver_PWMInit(MaxFPW,140);// Max FPW

		Driver_PWMInit(MaxBRK,1);// Max Brake


}

///////////////////////////////////  GAS   //////////////////////////////////

void Driver_SetThrottle (int Throttle)  		//Gasgeben
{

	static int ThPWM=0;
	state_akt=1;

	if((Throttle >= 0) && (Throttle <= 100))  	//Gasbereich begrenzen
	{
		ThPWM = MinFPW + res_throttle * Throttle; 	 // Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
	}
	if((ThPWM>=MinFPW) && (ThPWM<=MaxFPW))  		//Vorsichtsmaßnahme, damit wirklich keine falsche PWM gesetzt wird
	{
		TA1CCR1 = ThPWM;
	}

}


///////////////////////////////////  Bremse    //////////////////////////////////
void Driver_SetBrake (int Brake)    //Funktiom zum Bremsen
{

	static int BrPWM = 0;
	if((Brake >= 0) && (Brake <= 100))  	//Gasbereich begrenzen
	{
		if (state_akt == 1)
		{
			BrPWM = MinFPW - res_brk_f * Brake;  // Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
		}
		else if(state_akt==2)
		{
			BrPWM = MinRPW + res_brk_f * Brake;
		}
	}
	TA1CCR1 = BrPWM;
}



///////////////////////////////////  Rückwärts   //////////////////////////////////

void Driver_SetBack(int Backwards)         // Funktion zum Rückwärtsfahren
{
	state_akt = 2;
	static int BackPWM = 0;

	if((Backwards>=0) && (Backwards<=100))  	//Gasbereich begrenzen
	{
		BackPWM = MinRPW - res_back * Backwards; 	 // Pwm= PWM0Gas+ Throttle* (PWMMaxGas-PWM0Gas)/100
	}
	if((BackPWM>=MaxRPW) && (BackPWM<=MinRPW))  		//Vorsichtsmaßnahme, damit wirklich keine falsche PWM gesetzt wird
	{
		TA1CCR1 = BackPWM;
	}
}




///////////////////////////////////  Counter for ESC Init    //////////////////////////////////
void Driver_PWMInit(int PWM, int cycle)     // Wird für Initialisierung verwendet.
{
	initcounter=0;
	while(initcounter<=cycle)
	{
		TA1CCR1=PWM;
	}
}
