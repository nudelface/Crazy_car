
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
#include "HAL\hal_timerA0.h"
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




// 22 incremente


//int SteeringCalibC;
extern int SteeringCalibC;
extern ButtonCom Buttons;
extern USCIB1_SPICom SpiCom;
extern ADC12Com ADC1;
extern int state;
extern int counterz;
extern double PeriodTime;
extern double PeriodCount;
extern int SpeedReady;
extern int SpeedDir;


int AbstandRechts;
int AbstandLinks;
int AbstandFront;
int LastValueLeft=0;
int LastValueRight=0;
int LastValueFront=0;
double ESpeed=0;
double LastSpeed=0;
double LastESpeed=0;
int pwmOut=0;
double KpSpeed=0.8;
double KiSpeed=1;
double KdSpeed=1;
double SpeedDes=40
		;
double Speedf=0;
double Speed=0; //m/s
double LastiSpeed=0;
double iSpeed=0;

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

		if(SpeedReady==1)
		{
			LastSpeed=Speed;

			PeriodTime=(PeriodCount*0.016)/1000;
			Speedf=1/PeriodTime;
			Speed=5.06*Speedf/10;

			LastESpeed=ESpeed;
			ESpeed=SpeedDes-Speed;
			SpeedReady=0;
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

			DeltaDist=line_des+(AbstandRechts-AbstandLinks);
			LastDeltaDist=LastValueRight-LastValueLeft;
			dDelta=DeltaDist-LastDeltaDist;


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
			Driver_LCD_WriteString("speed",5,5,0);
			Driver_LCD_WriteUInt((int)Speed,5, 90);


			ADC1.Status.B.ADCrdy=0;
			counterz=0;
			if(StartupC<=1200)
			{
			StartupC++;
			}
		}



	if(DiskretEn==1)
	{
		switch(statecase)
		{
			case DriveStraight:

						ThrottleCorr=1;


						//LCD_BL_OFF;

				if((DeltaDist>3)||(DeltaDist<-3))
				{
				Steer=DeltaDist*kpSteer+dDelta*2.1;

					Driver_SetSteering(Steer);

				}



				///throttle control///
				if(drive==1)
				{

					if(AbstandFront>183)
					{
						SpeedDes=100;
					}
					else if(AbstandFront>100)
					{
						SpeedDes=50;
					}
					else if (AbstandFront>30)
					{
						SpeedDes=15;
					}

					iSpeed=(ESpeed*0.02)+LastiSpeed;
					LastiSpeed=iSpeed;
					pwmOut=KpSpeed*ESpeed+iSpeed*KiSpeed+(ESpeed-LastESpeed)*KdSpeed;

					if(pwmOut>0 && pwmOut<100)
					{
					Driver_SetThrottle(pwmOut);
					}
					else if (pwmOut<=-20 &&  pwmOut>-50)    // 0=vorw�rts????
					{
						if((dir==0) && (Speed>2))
							{Driver_SetBack(0);}
						else
						{Driver_SetBack(pwmOut);}
					}
					else if(pwmOut<=0 && pwmOut>-10)
					{
						Driver_SetThrottle(0);
					}
					else if(pwmOut>100)
					{
						Driver_SetThrottle(100);
					}
					else
					{
						Driver_SetBack(100);
					}


				}


				  if((AbstandFront<=50)&&(dFront>-20)&&StartupC>100)
					{
						Driver_SetBrake(1);
						statecase=Curve;

					}
					else if((AbstandFront<=50)&&(dFront<-20)&&StartupC>100)
					{
						statecase=Hinderniss;
						didit=1;
					}
					else if(AbstandFront<=20 && (AbstandLinks<15||AbstandRechts<15))
					{
						Driver_SetBrake(11);
						statecase=Hinderniss;


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
		DiskretEn=1;   ///////Achtung vllt eher doch auf 0 setzen!!!
	}


}
}
