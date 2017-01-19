
#include <msp430.h>
#include "HAL\hal_general.h"
#include "HAL\hal_gpio.h"
#include "HAL\Interrupts.h"
#include "HAL\hal_usc.h"
#include "HAL\hal_timerB0.h"
#include "DL\driver_general.h"
#include "DL\driver_aktorik.h"
#include "HAL\hal_usciB1.h"
#include "HAL\hal_adc12.h"
#include "HAL\hal_dma.h"
#include "DL\driver_lcd.h"
#include "AL\Sensorcalib.h"
#include "AL\sense_read.h"

/*
 * main.c
 */

float kpSteer=0.5;
float kpThrottle=0.3;
float ThrottleCorr=0;
int dir=0;  //1=links 2=rechts

//int SteeringCalibC;
extern int SteeringCalibC;
extern ButtonCom Buttons;
extern USCIB1_SPICom SpiCom;
extern ADC12Com ADC1;
extern int state;
extern int counterz;
int AbstandRechts;
int AbstandLinks;
int AbstandFront;
int LastValueLeft=0;
int LastValueRight=0;
int LastValueFront=0;

int dFront;
int dLeft;
int dRight;
int didit=0;

int StartupC=0;

int DiskretEn=0;

int drivestate=1;
int Steer=0;

int drive=0;
int DeltaDist=0;
int dDelta=0;
int line_des=0;
int LastDeltaDist=0;
int xFrontDist=0;

typedef enum {DriveStraight, Hinderniss, Curve} states;


states statecase=DriveStraight;






void main(void)
{


	        SteeringCalibC=-100;
			HAL_Init();
			Driver_Init();

	while(1)
	{

		if(Buttons.button==1&&Buttons.active==1)
		{
			if(drive==0)
			{
			LCD_BL_OFF;
			Buttons.button=0;
			Buttons.active=0;
			Driver_SetThrottle(0);
			drive=0;
			}
			else if(drive==1)
			{
				Buttons.button=0;
				Buttons.active=0;
				//kpSteer=kpSteer-0.1;
			}

		}
		else if (Buttons.button==2&&Buttons.active==1)
		{
			if(drive==0)
			{
			LCD_BL_ON;
			Driver_SetSteering(0);
			Driver_SetThrottle(60);
			drive=1;
			Buttons.button=0;
			Buttons.active=0;
			}
			else
			{
				Buttons.button=0;
				Buttons.active=0;
				//kpSteer=kpSteer+0.1;
			}
		}


		if(ADC1.Status.B.ADCrdy==1)
		{
			LastValueLeft=AbstandLinks;
			LastValueRight=AbstandRechts;
			LastValueFront=AbstandFront;


			AbstandRechts=Distright(ADC1.Bit_right);
		    AbstandLinks=Distleft(ADC1.Bit_left);
		    AbstandFront=Distmid(ADC1.Bit_front);

			dLeft=AbstandLinks-LastValueLeft;
			dRight=AbstandRechts-LastValueRight;
			dFront=AbstandFront-LastValueFront;


			Driver_LCD_WriteString("dist_left",6,1,0);
			Driver_LCD_WriteUInt((int)AbstandLinks,1, 50);
			Driver_LCD_WriteString("dLeft",5,1,70);
			if(dFront<=0)
				{
				Driver_LCD_WriteString("-",1,1,100);
				Driver_LCD_WriteUInt(-dFront,1,108);
				}
			else
				{
				Driver_LCD_WriteString("+",1,1,100);
				Driver_LCD_WriteUInt(dFront,1,108);
			}
			Driver_LCD_WriteString("V_right",7,2,0);
			Driver_LCD_WriteUInt((int)AbstandRechts,2, 50);
			Driver_LCD_WriteString("V_front",7,4,0);
			Driver_LCD_WriteUInt(AbstandFront,4, 50);
			Driver_LCD_WriteString("V_Batt",6,5,0);
			Driver_LCD_WriteUInt(ADC1.vBat,5, 90);


			ADC1.Status.B.ADCrdy=0;
			counterz=0;
			if(StartupC<=1200)
			{
			StartupC++;
			}
		}

/*
		if (drive==1 &&( ADC1.Bit_front>1600 ||ADC1.Bit_right>2500 ||ADC1.Bit_left>2500) )
		{
			Driver_SetBack(80);

			if(ADC1.Bit_left>ADC1.Bit_right-40)
			{
			Driver_SetSteering(-100);} //rechts}
			else if(ADC1.Bit_left<ADC1.Bit_right+40)
			{Driver_SetSteering(100);}

		}

		else if(drive==1&&ADC1.Bit_front>900 )
			{
				Driver_SetThrottle(45);

				if(ADC1.Bit_left>ADC1.Bit_right-50)
				{
				Driver_SetSteering(100);} //rechts}
				else
				{Driver_SetSteering(-100);}
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

				}*/

	if(DiskretEn==1)
	{
		switch(statecase)
		{
			case DriveStraight:

						ThrottleCorr=1;


						//LCD_BL_OFF;
			DeltaDist=line_des+(AbstandRechts-AbstandLinks);
			LastDeltaDist=LastValueRight-LastValueLeft;
			dDelta=DeltaDist-LastDeltaDist;
				if((DeltaDist>3)||(DeltaDist<-3))
				{
				Steer=DeltaDist*kpSteer+dDelta*2.1;

					Driver_SetSteering(Steer);

				}



				///throttle control///
				if(drive==1)
				{

					if(dFront<-10)
					{
						Driver_SetBack(100);
					}
					if((AbstandFront>=183)&&(DeltaDist>-10||DeltaDist<10))
					{
						Driver_SetThrottle(100);


					}
					else if (((AbstandFront<=183)&&AbstandFront>120)&&(dFront<-10))
					{
						Driver_SetBack(100);
					}
					else if ((AbstandFront>=150)&&(DeltaDist<=-10||DeltaDist>=10))
					{
						Driver_SetThrottle(55);

					}
					else if(AbstandFront>50)
					{
						if(((((AbstandFront-49)*0.1)+50)<=60))
						Driver_SetThrottle((((AbstandFront-49)*0.1)+53));
						else
						Driver_SetThrottle(60);

					}
					else if((AbstandFront<=50)&&(dFront>-40)&&StartupC>100)
					{
						Driver_SetBrake(1);
						statecase=Curve;

					}
					/*else if((AbstandFront<=50)&&(dFront<-8)&&StartupC>100)
					{
						statecase=Hinderniss;
						didit=1;
					}*/
					else if(AbstandFront<=20 && (AbstandLinks<15||AbstandRechts<15))
					{
						Driver_SetBrake(11);
						statecase=Hinderniss;


					}
				}

        if (StartupC>100)
        {

				/// Abbruch Straight fo into curve
				if((dLeft>20 && AbstandLinks<50) || (dLeft>10 && (AbstandLinks>=50)))
				{
					Driver_SetBrake(1);
					dir=1;   // Links
					statecase= Curve;
				}



				else if( (dRight>20 && AbstandRechts<50) || (dRight>10 && (AbstandRechts>=50)) )
				{
					Driver_SetBrake(1);
						dir=2;  // Rechts
						statecase= Curve;
				}


        }
			break;

			case Curve:
				LCD_BL_ON;
				if(dir==1)
				{	Driver_SetSteering(-100);
					if((AbstandFront>AbstandLinks)&&(AbstandFront>120))
					{statecase=DriveStraight;}
					else if(dRight<-20)
					{dir=2;}
					Driver_SetThrottle((((AbstandFront-50)*0.15)+53));
				}

				else if(dir==2)
				{	Driver_SetSteering(100);
					if((AbstandFront>AbstandRechts)&&(AbstandFront>120))
					{statecase=DriveStraight;}
					else if(dLeft<-20)
					{dir=1;}
						Driver_SetThrottle((((AbstandFront-50)*0.15)+53));
				}

				if(AbstandFront<=10 && (AbstandLinks<10||AbstandRechts<10))
				{
					Driver_SetBrake(100);
					statecase=Hinderniss;

				}

			break;

			case Hinderniss:

			if(didit==1)
			{
				//line_des=20;
				DeltaDist=line_des-(AbstandRechts-AbstandLinks);
				LastDeltaDist=LastValueRight-LastValueLeft;
				dDelta=DeltaDist-LastDeltaDist;
					if((DeltaDist>3)||(DeltaDist<-3))
					{
					Steer=DeltaDist*kpSteer+dDelta*2.1;

						Driver_SetSteering(Steer);

					}
					else
					{
						didit=0;
						statecase=DriveStraight;
					}
			}

			if(AbstandFront<20)
			{
				if(AbstandLinks<AbstandRechts)
				{
					Driver_SetSteering(-100);
					Driver_SetBack(80);
				}
				else if (AbstandRechts<=AbstandLinks)
				{
					Driver_SetSteering(100);
					Driver_SetBack(80);
				}
			}

				if(AbstandFront>10)
				{statecase=DriveStraight;}

            break;

		}
		DiskretEn=0;
	}


}
}
