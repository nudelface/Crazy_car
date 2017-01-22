
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
#include "AL\Controller.h"

/*
 * main.c
 */

const float kpSteer=0.5;
const float kpThrottle=0.3;

int dir=0;  //1=links 2=rechts
int icounter=0;




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
extern int Timeout;


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

double SpeedDes=0;
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


int Steer=0;

int drive=0;
int DeltaDist=0;
int dDelta=0;
int line_des=0;
int LastDeltaDist=0;


typedef enum {DriveStraight, Hinderniss, Curve} states;
typedef enum {UTurnL,UTurnR,BadRTurn, LTurn, RTurn} turns;


states statecase=DriveStraight;
states laststate=DriveStraight;
turns  Corner=LTurn;








void main(void)
{


	        SteeringCalibC=-100;
			HAL_Init();
			Driver_Init();

	while(1)
	{

		if(Buttons.button==1&&Buttons.active==1)
		{

			LCD_BL_OFF;
			Buttons.button=0;
			Buttons.active=0;
			Driver_SetThrottle(40);
			drive=0;



		}
		else if (Buttons.button==2&&Buttons.active==1)
		{

			LCD_BL_ON;
			Driver_SetSteering(0);
			drive=1;
			Driver_SetThrottle(40);
			Buttons.button=0;
			Buttons.active=0;

		}

		if(SpeedReady==1 && drive==1)
		{
			LastSpeed=Speed;

			PeriodTime=(PeriodCount*0.016)/1000;
			Speedf=1/PeriodTime;
			if(PeriodCount<=50000)
			{
			if(SpeedDir==0)
			{Speed=5.06*Speedf/10;}
			else
			{
				Speed=-5.06*Speedf/10;}
			}
			else
			{
				Speed=0;
			}
			LastESpeed=ESpeed;
			ESpeed=SpeedDes-Speed;
			SpeedReady=0;


			///throttle control///
							if(drive==1)
							{


								if(icounter<=500)
								{
								iSpeed=(ESpeed*0.02)+LastiSpeed;
								icounter++;
								}
								else
								{
									iSpeed=ESpeed*0.02*2;
									icounter=0;
								}
								LastiSpeed=iSpeed;
								pwmOut=KpSpeed*ESpeed+iSpeed*KiSpeed+(ESpeed-LastESpeed)*KdSpeed;

								//if((ESpeed<-15) && Speed >50)
								//{
								//	Driver_SetBack(100);
								//}
								if(dFront<20)
								{Driver_SetBack(100);

								}
								else if(pwmOut>0 && pwmOut<100)
								{
								Driver_SetThrottle(pwmOut);
								}
								else if ((pwmOut<=-15 &&  pwmOut>-200))    // 0=vorw�rts????
								{
									//if((dir>0) && (Speed>10))
									//	{Driver_SetBack(0);}
									//else
									Driver_SetBack(100);
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
							else
							{
								Driver_SetThrottle(0);
							}

		}
		else if (SpeedReady==1)
		{
			Speed=0;
			LastSpeed=0;
			ESpeed=0;
			LastESpeed=0;
		}
		else if(SpeedReady==0)
		{
			Speed=LastSpeed;
			if(drive==1)
									{


											if(icounter<=500)
											{
											iSpeed=(ESpeed*0.02)+LastiSpeed;
											icounter++;
											}
											else
											{
												iSpeed=ESpeed*0.02*2;
												icounter=0;
											}
											LastiSpeed=iSpeed;
											pwmOut=KpSpeed*ESpeed+iSpeed*KiSpeed+(ESpeed-LastESpeed)*KdSpeed;

											//if((ESpeed<-15) && Speed >50)
											//{
											//	Driver_SetBack(100);
											//}
											if(dFront<-20)
											{Driver_SetBack(100);

											}
											else if(pwmOut>0 && pwmOut<100)
											{
											Driver_SetThrottle(pwmOut);
											}
											else if ((pwmOut<=-15 &&  pwmOut>-200))    // 0=vorw�rts????
											{
												//if((dir>0) && (Speed>10))
												//	{Driver_SetBack(0);}
												//else
												Driver_SetBack(100);
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
										else
										{
											Driver_SetThrottle(0);
										}

		}

		if(ADC1.Status.B.ADCrdy==1)
		{

			if (Timeout<=20)
			{
			Timeout++;
			}
			else
			{
				Speed=0;
				LastSpeed=0;
				ESpeed=0;
				LastESpeed=0;
				Timeout=0;
			}
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

			if(drive==0)
			{
					Driver_LCD_WriteString("dist_left",6,1,0);
					Driver_LCD_WriteUInt((int)AbstandLinks,1, 50);
					Driver_LCD_WriteString("dir",3,1,70);
					if(SpeedDir<=0)
						{
						//Driver_LCD_WriteString("-",1,1,100);
						Driver_LCD_WriteUInt(SpeedDir,1,108);
						}
					else
						{
						Driver_LCD_WriteString("+",1,1,100);
						Driver_LCD_WriteUInt(SpeedDir,1,108);
					}
					Driver_LCD_WriteString("V_right",7,2,0);
					Driver_LCD_WriteUInt((int)AbstandRechts,2, 50);
					Driver_LCD_WriteString("V_front",7,4,0);
					Driver_LCD_WriteUInt(AbstandFront,4, 50);
					Driver_LCD_WriteString("speed",5,5,0);
					Driver_LCD_WriteUInt((int)Speed,5, 90);
			}


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

				if(laststate!=statecase)
				{
					laststate=statecase;
					Driver_LCD_Clear();
					Driver_LCD_WriteString("DriveStraight",13,5,0);


				}
				line_des=0;

				if(AbstandFront>183)
				{
					SpeedDes=120;
				}
				else if (AbstandFront> 160)
				{
					SpeedDes=50;
				}
				else if(AbstandFront>100)
				{
					SpeedDes=40;
				}
				else if (AbstandFront>30)
				{
					SpeedDes=40;
				}
				else
				{
					SpeedDes=40;
				}



				if((DeltaDist>3)||(DeltaDist<-3))
				{
				Steer=DeltaDist*kpSteer+dDelta*2.1;

					Driver_SetSteering(Steer);

				}


				if((AbstandFront<=50)&&(dFront<-20)&&(StartupC>100))
				{
					statecase=Hinderniss;
					didit=1;
				}
				else if((AbstandFront<=8 )&& (AbstandLinks<3||AbstandRechts<3)&&(Speed<=5))
				{
					statecase=Hinderniss;
				}



				 /* if((AbstandFront<=50)&&(dFront<-20)&&(StartupC>100)&&((DeltaDist<10)||(DeltaDist>-10)))
					{
						statecase=Hinderniss;
						didit=1;
					}
					else if((AbstandFront<=5)&&((dFront>-10)||(dFront<10)))
					{
						statecase=Hinderniss;
						didit=1;
					}
					else if((AbstandFront<50)&&((DeltaDist>-10)||(DeltaDist<10)))
					{
							statecase=Hinderniss;
							didit=1;
					}
					else if((AbstandFront<=8 )&& (AbstandLinks<3||AbstandRechts<3))
					{
						//Driver_SetBack(100);
						statecase=Hinderniss;
					}*/


        if (StartupC>100)
        {

				/// Abbruch Straight fo into curve
				if((dLeft>20) || (dLeft>10 && AbstandFront>170))
				{
					//Driver_SetBack(100);
					SpeedDes=55;
					Corner=LTurn;   // Links
					statecase= Curve;
				}



				else if((dRight>20) || (dRight>10 && AbstandFront >170))
				{
					//Driver_SetBack(100);
					SpeedDes=55;
						Corner=RTurn;  // Rechts
						statecase= Curve;
				}


        }
			break;



			case Curve:
				LCD_BL_ON;
				if(laststate!=statecase)
				{
					if((AbstandFront>=130)&&(Corner==LTurn))
					{
						Corner=UTurnL;
						Driver_LCD_Clear();
						Driver_LCD_WriteString("UTurnL",6,5,0);
					}
					else if((AbstandFront>=130)&&(Corner==RTurn))
					{
						Corner=UTurnR;
						Driver_LCD_Clear();
						Driver_LCD_WriteString("UTurnR",6,5,0);
					}
					else
					{
						Driver_LCD_Clear();
						Driver_LCD_WriteString("Corner",6,5,0);
					}
					//l�ngen merken, ob gro�e u kurve oder nicht
					laststate=statecase;
				}

				////
				if(AbstandLinks<=3)
				{
					statecase=Hinderniss;
				}
				else if (AbstandRechts<=3)
				{
					statecase=Hinderniss;
				}

///
				if(Corner==LTurn)
				{
					Driver_SetSteering(-100);
					if((AbstandFront>AbstandLinks)&&(AbstandFront>140))
					{statecase=DriveStraight;}
					else if(dRight>15)
					{dir=RTurn;}

					SpeedDes=45;
				}
				else if(Corner==RTurn)
				{

					Driver_SetSteering(100);
					if((AbstandFront>AbstandRechts)&&(AbstandFront>140))
					{statecase=DriveStraight;}
					else if(dLeft>15)
					{dir=LTurn;}

					SpeedDes=45;
				}
				else if(Corner==UTurnR)
				{
					Driver_SetSteering(100);
					if((AbstandFront>AbstandRechts)&&(AbstandFront>150)&&(AbstandLinks<=70)&&(AbstandRechts<=70))
					{statecase=DriveStraight;}
					else if((AbstandRechts<25)&&(AbstandFront>110))
					{
						statecase=DriveStraight;
						Driver_SetSteering(0);
					}
					else if((AbstandLinks<=8)&&(AbstandFront>14))
					{
						SpeedDes=70;
						statecase=DriveStraight;
					}

					else if(AbstandLinks<=5)
					{
						statecase=Hinderniss;
					}

					SpeedDes=45;
				}

				else if(Corner==UTurnL)
				{
					Driver_SetSteering(-100);

					if((AbstandFront>AbstandLinks)&&(AbstandFront>150)&&(AbstandLinks<=70)&&(AbstandRechts<=70))
					{statecase=DriveStraight;}
					else if((AbstandLinks<25)&&(AbstandFront>110))
					{
						statecase=DriveStraight;
						Driver_SetSteering(0);
					}
					else if((AbstandRechts<=8)&&(AbstandFront>14))
					{
						SpeedDes=70;
						statecase=DriveStraight;
					}
					else if(AbstandRechts<=5)
					{
						statecase=Hinderniss;
					}
					SpeedDes=45;
				}



				if(AbstandFront<=10 && (AbstandLinks<10||AbstandRechts<10))
				{
					Driver_SetBack(50);
					//Driver_SetBrake(100);
					statecase=Hinderniss;

				}
				else if((AbstandLinks<5)||(AbstandRechts<5))
				{
					Driver_SetBack(50);
					//Driver_SetBrake(100);
					statecase=Hinderniss;
				}


			break;

			case Hinderniss:

				Driver_LCD_Clear();
				Driver_LCD_WriteString("Hinderniss",10,5,0);

				if(laststate==DriveStraight)
				{
					if(didit==1)
					{
									SpeedDes=60;
									line_des=20;

											if((DeltaDist>3)||(DeltaDist<-3))
											{
												Steer=DeltaDist*kpSteer+dDelta*2.1;
												Driver_SetSteering(Steer);

											}
											else
											{
												line_des=0;
												didit=0;

												if(laststate!=statecase)
												{
													laststate=statecase;
												}
												statecase=DriveStraight;
											}
					}


					else
					{

							if(AbstandFront<10)
							{
								if(AbstandLinks<AbstandRechts)
								{
									Driver_SetSteering(-100);

									Driver_SetBack(50);
								}
								else if (AbstandRechts<=AbstandLinks)
								{
									Driver_SetSteering(100);

									Driver_SetBack(50);
								}
							}
							else
							{
								line_des=0;
								didit=0;

								if(laststate!=statecase)
								{
									laststate=statecase;
								}
								statecase=DriveStraight;
							}
						}

					}



				else if(laststate==Curve)
				{
					if(AbstandFront<10)
					{
						if(AbstandLinks<AbstandRechts)
						{
							Driver_SetSteering(-100);
							Driver_SetBack(50);
							//Driver_SetBack(80);
						}
						else if (AbstandRechts<=AbstandLinks)
						{
							Driver_SetSteering(100);
							Driver_SetBack(50);
							//Driver_SetBack(80);
						}
					}
					else
						{
						line_des=0;
						didit=0;

						if(laststate!=statecase)
						{
							laststate=statecase;
						}
						statecase=DriveStraight;
						}
					}


				if(AbstandFront>20)
				{

					if(laststate!=statecase)
					{
						laststate=statecase;
					}
					statecase=DriveStraight;
				}



            break;

		}
		DiskretEn=1;   ///////Achtung vllt eher doch auf 0 setzen!!!
	}


}
}
