

#include "Driver/driver_aktorik.h"
#include "HAL\hal_general.h"
#include "HAL\hal_gpio.h"
#include "HAL\Interrupts.h"
#include "HAL\hal_usc.h"
#include "HAL\hal_timerB0.h"
#include "Driver\driver_general.h"
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
	SteeringCalibC=-100;
			HAL_Init();
			Driver_Init();
			LCD_BL_ON;


	while(1)
	{



		if(SteeringCalibC<100)
		{
			Driver_SetSteering(SteeringCalibC);
		}
		else if (SteeringCalibC>=100)
		{
			Driver_SetSteering(80);
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
