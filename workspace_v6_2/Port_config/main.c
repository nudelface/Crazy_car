

#include "HAL\hal_general.h"
#include "HAL\hal_gpio.h"
#include "HAL\Interrupts.h"
#include "HAL\hal_usc.h"
#include "HAL\hal_timerB0.h"
#include "Driver\driver_general.h"
#include "Driver\driver_aktorik.h"
#include "HAL\hal_usciB1.h"


/*
 * main.c
 */

//int SteeringCalibC;
extern int SteeringCalibC;
extern ButtonCom Buttons;
extern USCIB1_SPICom SpiCom;





void main(void)
{
	SteeringCalibC=-560;
			HAL_Init();
			Driver_Init();
			LCD_BL_ON;


	while(1)
	{

		if(Buttons.button==1&&Buttons.active==1)
		{
			LCD_BL_ON;
			Buttons.button=0;
			Buttons.active=0;
		}
		else if (Buttons.button==2&&Buttons.active==1)
		{
			LCD_BL_OFF;
			Buttons.button=0;
			Buttons.active=0;
		}

		if(SteeringCalibC<560)
		{
			Driver_SetSteering(SteeringCalibC);
		}
		else if (SteeringCalibC==560)
		{
			Driver_SetSteering(0);
		}
		if(SpiCom.Status.B.TXSuc==1)
		{
		HAL_USCIB1_Transmit();
		}
//	LCD_BL_OFF;
//	z=a+b;
//LCD_BL_ON;

	}
}
