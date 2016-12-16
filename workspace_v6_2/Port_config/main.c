

#include "HAL\hal_general.h"
#include "HAL\hal_gpio.h"
#include "HAL\Interrupts.h"
#include "HAL\hal_usc.h"
#include "HAL\hal_timerB0.h"
#include "Driver\driver_general.h"
#include "Driver\driver_aktorik.h"
#include "HAL\hal_usciB1.h"
#include "HAL\hal_adc12.h"
#include "HAL\hal_dma.h"
#include "Driver\driver_lcd.h"
#include "AL\interp.h"

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
float Abstand;

int drivestate=1;

int drive=0;


int Values_right[21]={80,75,70,65,60,55,50,45,40,35,30,25,20,15,10,7,5,4,3,2,0};
int Bit_right[21]={400,423,465,486,507,550,594,636,690,780,865,970,1121,1350,1674,1998,2307,2465,2658,2918,3353};




///unsigned char cmdArr[9]= {LCD_RESET, LCD_BIAS, ADC_SEL_NORMAL, COMMON_REVERSE, RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, POWER_CONT, DISPLAY_ON};

void main(void)
{


	SteeringCalibC=-560;
			HAL_Init();
			Driver_Init();


Abstand= interp(Bit_right, Values_right, 1050, 21);

	while(1)
	{

		if(Buttons.button==1&&Buttons.active==1)
		{
			//Driver_LCD_WriteCommand(&cmdArr[0], 5);

			LCD_BL_OFF;
			Buttons.button=0;
			Buttons.active=0;
			Driver_SetThrottle(0);
			drive=0;

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
			Driver_LCD_WriteString("V_left",6,1,0);
			Driver_LCD_WriteUInt(ADC1.Bit_left,1, 50);
			Driver_LCD_WriteString("V_right",7,2,0);
			Driver_LCD_WriteUInt(ADC1.Bit_right,2, 50);
			Driver_LCD_WriteString("V_front",7,4,0);
			Driver_LCD_WriteUInt(ADC1.Bit_front,4, 50);
			Driver_LCD_WriteString("V_Batt",6,5,0);
			Driver_LCD_WriteUInt(ADC1.vBat,5, 90);
			ADC1.Status.B.ADCrdy=0;
			counterz=0;
		}


		if (drive==1 &&( ADC1.Bit_front>1600 ||ADC1.Bit_right>2500 ||ADC1.Bit_left>2500) )
		{
			Driver_SetBack(80);

			if(ADC1.Bit_left>ADC1.Bit_right-40)
			{
			Driver_SetSteering(-560);} //rechts}
			else if(ADC1.Bit_left<ADC1.Bit_right+40)
			{Driver_SetSteering(560);}

		}

		else if(drive==1&&ADC1.Bit_front>900 )
			{
				Driver_SetThrottle(45);

				if(ADC1.Bit_left>ADC1.Bit_right-50)
				{
				Driver_SetSteering(560);} //rechts}
				else
				{Driver_SetSteering(-560);}





//	LCD_BL_OFF;
//	z=a+b;
//LCD_BL_ON;
			}

		else if(drive==1 && ADC1.Bit_front<900)
		{
			Driver_SetThrottle(70);
			Driver_SetSteering(0);

		}
				else if (drive==1 && ADC1.Bit_front<700)
				{
					Driver_SetThrottle(80);
					Driver_SetSteering(0);

				}

	}


}
