

#include "HAL\hal_general.h"
#include "HAL\hal_gpio.h"
#include "HAL\Interrupts.h"
#include "HAL\hal_usc.h"
#include "HAL\hal_timerB0.h"
#include "Driver\driver_general.h"
#include "Driver\driver_aktorik.h"
#include "HAL\hal_usciB1.h"
#include "HAL\hal_adc12.h"
#include "Driver\driver_lcd.h"

/*
 * main.c
 */

//int SteeringCalibC;
extern int SteeringCalibC;
extern ButtonCom Buttons;
extern USCIB1_SPICom SpiCom;
extern ADC12Com ADC1;
extern int state;
extern int counterz;

int drive=0;


///unsigned char cmdArr[9]= {LCD_RESET, LCD_BIAS, ADC_SEL_NORMAL, COMMON_REVERSE, RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, POWER_CONT, DISPLAY_ON};

void main(void)
{

	SteeringCalibC=-560;
			HAL_Init();
			Driver_Init();



	while(1)
	{

		if(Buttons.button==1&&Buttons.active==1)
		{
			//Driver_LCD_WriteCommand(&cmdArr[0], 5);

			LCD_BL_OFF;
			Buttons.button=0;
			Buttons.active=0;
			Driver_SetThrottle(0);

			//Driver_LCD_WriteUInt(255,3, 0);
		}
		else if (Buttons.button==2&&Buttons.active==1)
		{
			LCD_BL_ON;
			Driver_SetSteering(0);
			Driver_SetThrottle(60);
			drive=1;
			Buttons.button=0;
			Buttons.active=0;

		}


		if(ADC1.Status.B.ADCrdy==1)
		{
			Driver_LCD_WriteString("V_r",14,1,0);
			Driver_LCD_WriteUInt(ADC1.Bit_left,1, 18);
			Driver_LCD_WriteUInt(ADC1.Bit_right,2, 10);
			Driver_LCD_WriteUInt(ADC1.Bit_front,4, 19);
			Driver_LCD_WriteUInt(ADC1.Bit_front,5, 19);
			counterz=0;
		}

		if (drive=1 && ADC1.Bit_front>1500)
		{
			Driver_SetBrake(100);
			drive=0;
		}

//	LCD_BL_OFF;
//	z=a+b;
//LCD_BL_ON;

	}
}
