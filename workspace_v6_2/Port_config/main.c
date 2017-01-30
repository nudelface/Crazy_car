
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

const float Excel=40800;
float KPID=0;
float lastKPID=0;


const float KpSpeed=1.4;
const float KiSpeed=0.3;
const float KdSpeed=0.2;




// 22 incremente


//int SteeringCalibC;

extern ButtonCom Buttons;
extern USCIB1_SPICom SpiCom;
extern ADC12Com ADC1;
extern int state;
extern int counterz;
extern double PeriodTime;
extern unsigned long PeriodCount;
extern int SpeedReady;
extern int SpeedDir;
extern int Timeout;


int AbstandRechts;
int AbstandLinks;
int AbstandFront;
int LastValueLeft=0;
int LastValueRight=0;
int LastValueFront=0;
float ESpeed=0;
float LastSpeed=0;
float LastESpeed=0;
int pwmOut=0;

float SpeedDes=0;
float LastSpeedDes=0;


float Speed=0; //m/s
float LastiSpeed=0;
float iSpeed=0;

int dFront;
int dLeft;
int dRight;
int didit=0;

int StartupC=0;


int HardBraking=0;


int Steer=0;

int drive=0;
int DeltaDist=0;
int dDelta=0;
int line_des=0;
int LastDeltaDist=0;
extern int SpeedSamp;


typedef enum {DriveStraight, Hinderniss, Curve} states;
typedef enum {UTurnL,UTurnR,BadRTurn, LTurn, RTurn} turns;


states statecase=DriveStraight;
states laststate=Curve;
turns  Corner=LTurn;








void main(void)


{
			HAL_Init();
			Driver_Init();

	while(1)
	{


	/*	while(1)
		{
			if(drive==1)
			{

			}
		}*/
		//////////////////////////////Button Read
		if(Buttons.button==1&&Buttons.active==1)  //black button
		{


			Driver_SetSteering(0);
			drive=1;
			statecase=DriveStraight;
			Buttons.button=0;
			Buttons.active=0;



		}
		else if (Buttons.button==2&&Buttons.active==1)  //red button
		{




			Buttons.button=0;
			Buttons.active=0;
			SpeedDes=0;
			drive=0;

		}


		//////////////////////////////////////////////////Init des Regler bei desired �nderung
		if(LastSpeedDes!=SpeedDes)
		{
			iSpeed=0;
			iSpeed=0;
			LastiSpeed=0;
			LastSpeed=0;
			Timeout=0;
			LastESpeed=0;
			ESpeed=SpeedDes-Speed;
			KPID=KpSpeed*ESpeed;
			lastKPID=0;
			LastSpeedDes=SpeedDes;
		}





		///Abstandermittlung und Berechneng f�r LENK-PID//////////////////////////////////////////////////////////////////////77

		if(ADC1.Status.B.ADCrdy==1)
			{


			//SpeedMess TImeout Setzt speed usw auf 0, wenn Rad sich nicht dreht

			if ((Timeout<=5)&&(HardBraking==0))  //in fahrt muss erst nach 1s stillstand erkannt werden
			{
			Timeout++;
			}
			else if ((Timeout<=2)&&(HardBraking==1))  //in Bremse muss blockieren detektiert werden
			{
				Timeout++;

			}
			else
			{
				Speed=0;
				iSpeed=0;
				LastiSpeed=0;
				LastSpeed=0;
				Timeout=0;
				LastESpeed=0;
				ESpeed=SpeedDes-Speed;
				KPID=KpSpeed*ESpeed;
				lastKPID=0;
			}
			///////

					LastValueLeft=AbstandLinks;
					LastValueRight=AbstandRechts;
					LastValueFront=AbstandFront;


					AbstandRechts=Distright(ADC1.Bit_right);
				    AbstandLinks=Distleft(ADC1.Bit_left);
				    AbstandFront=Distmid(ADC1.Bit_front);

					dLeft=AbstandLinks-LastValueLeft;
					dRight=AbstandRechts-LastValueRight;
					dFront=AbstandFront-LastValueFront;



					if(statecase!=Hinderniss)
					{
					LastDeltaDist=DeltaDist;
					DeltaDist=(AbstandRechts-AbstandLinks);  //deltaDist = e

					dDelta=DeltaDist-LastDeltaDist;          //dDelta = e abgeleitet
					}


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


					if(StartupC<=1200) ///zur Sp�teren 0-wert bei startab ignorierung
					{
					StartupC++;
					}
				}





		if(((LastESpeed>0)&&(ESpeed<0))||((LastESpeed<0)&&(ESpeed>0)))
		{
			iSpeed=KPID*0.002;
			LastiSpeed=iSpeed;
		}
		else
		{
			iSpeed=(KPID*0.002)+LastiSpeed;
			LastiSpeed=iSpeed;
			if((iSpeed>=500)&&(SpeedDes>5))
			{
				iSpeed=500;
			}
			else if((SpeedDes<-10)&&(iSpeed<-500))
			{
				iSpeed=-500;
			}
			else if((iSpeed>100)&&(SpeedDes<-5))
			{
				iSpeed=0;
			}
		}

		///Speed Controller///////////////////////////////////Rennt auf Abtastung von Speed-Measurement


		//Calculating new Values
		if((SpeedReady==1))  //neue Werte ermittelt und in Drive
		{
			LastSpeed=Speed;  //LastSpeed beschreiben
			//PeriodTime=(PeriodCount*0.00016); //Periodendauer in s*10 (damit nachher cm/s anstatt mm/s


			if(SpeedDir<=0)  //Vorw�rts
				{

					Speed=Excel/PeriodCount;
					if((Speed>LastSpeed+5)&&(Speed>SpeedDes*0.75))
					{
						Speed=LastSpeed+15;
					}
					else if((Speed<LastSpeed-5)&&(Speed>SpeedDes*0.75))
					{
						Speed=LastSpeed-15;
					}
				}
			else               //R�ckw�rts
				{
					Speed=-Excel/PeriodCount;
				}

			LastESpeed=ESpeed;
			ESpeed=SpeedDes-Speed;
			lastKPID=KPID;
			KPID=KpSpeed*ESpeed;
			 //Regelabweichung
			// NEew PED-Value
			//pwmOut=KpSpeed*ESpeed+iSpeed*KiSpeed+(ESpeed-LastESpeed)*KdSpeed;  //PID
			SpeedReady=0;
		}
		else if(drive==0)  //wenn off
		{
			SpeedDes=0;
		}




		if(drive==1)
		{

			if(SpeedDes>=0)
			{
			pwmOut=(int)(KPID+((iSpeed)*KiSpeed)+(KPID-lastKPID)*KdSpeed)+40;
			}
			else
			{
				pwmOut=-60;
			}

			//pwmOut=KpSpeed*ESpeed*(1+(200/iSpeed)*KiSpeed+ (ESpeed-LastESpeed)*KdSpeed;)
		 //PID



		// Stellglied
		//if(dFront<20)        // Wenn Wand schnell n�her kommt abbremsen  !!!!!!!!!!!!!!!!!!!CHECKEN OBS �BERHAUPT WAS BRINGT
		//{
		//	Set
		//	HardBraking=0;
		//}

		 if(pwmOut>0 && pwmOut<100)     ///WEnn PWM zwischen 0 und 100 prozent
		{
			if(pwmOut<40&&pwmOut>5)
			{
				Driver_SetThrottle(40);
			}

		Driver_SetThrottle(pwmOut);
		}
		else if((pwmOut<=0 && pwmOut>-40)&&(SpeedDes>5)) //bei negativer PWM
		{
			Driver_SetThrottle(0);
		}
		else if ((pwmOut<=-40) &&  (pwmOut>-100) &&(Speed>1))    // NEgative PWM
		{
			Driver_SetThrottle(0);
			//Driver_SetBack(-pwmOut);
			HardBraking=0;
			//Ehemals
			//Driver_SetBack(100);
		}
		else if((pwmOut<=-40) &&  (pwmOut>-100) &&(SpeedDes<-1))
		{
			Driver_SetBack(-pwmOut);
		}
		else if(pwmOut>100)         //Grenzwert
		{
			Driver_SetThrottle(100);
		}
		else  if(pwmOut<-100)                //HardBrake
		{
			Driver_SetBack(-pwmOut>>1);
			HardBraking=0;
		}
		else if(pwmOut<-200)
		{
			Driver_SetBack(100);
			HardBraking=0;
		}
		else
		{
			Driver_SetThrottle(0);
		}
		}
		else if(drive==0)
		{
			Driver_SetThrottle(0);
		}


		////////////////////////////////////////////////////////////////////

		//STATE MACHINE ////////////////////////////////////////////////////

		switch(statecase)
		{
			case DriveStraight:
				LCD_BL_OFF;
				if(laststate!=statecase)  //komme ich aus DriveStraight?
				{
					laststate=statecase;
					////Driver_LCD_Clear();
					//Driver_LCD_WriteString("DriveStraight",13,5,0);
					line_des=0;

				}
				//line_des=0;

			if(drive==1)
			{
				if(AbstandFront>183) ////geschwindkeitswahl
				{
					SpeedDes=250;

				}
				else if ((AbstandFront> 150)&&(dFront<-2))
				{
					SpeedDes=80;

				}

				else if((AbstandFront>100)&&(dFront<-8))
				{
					SpeedDes=60;

				}
				else if((AbstandFront>100)&&(dFront<-2))
				{
					SpeedDes=70;
				}
				else if((AbstandFront>100)&&(dFront>=0))
				{
					SpeedDes=140;
				}
				else if (AbstandFront>60)
				{
					SpeedDes=80;
				}
				else
				{
					SpeedDes=50;
				}

			}




				if((AbstandFront<=50)&&(dFront<15||dFront>-15)&&(Speed>60))  //wenn Abstand Vorne kleiner als 50cm ist UND Objekt abstand sich kaum �ndert
				{
					line_des=10; //�berholen
                                    //ausweich/�berholman�ver
				}
				else if((AbstandFront<=8 )&& (AbstandLinks<3||AbstandRechts<3)&&(Speed<=5))  //Wenn AbstandFront<8 und Abstand Links gering und Speed gering, dann r�ckw�rts
				{
					statecase=Hinderniss;
				}
				else if (AbstandFront<5)
				{
					statecase=Hinderniss;
				}


		        if (StartupC>100)  //startuptimer, damit nicht falsch gemessen wird   KURVENDETEKTION
		        {
						/// Abbruch Straight to into curve
		/*dLEft und right waren auf 10*/				if((dLeft>9) || (dLeft>10 && AbstandFront>170)||(AbstandLinks>AbstandRechts+50))   //Links pl�tzlich weg, linkskurve
						{
							//Driver_SetBack(100);

							Corner=LTurn;   // Links
							statecase= Curve;
							Driver_SetSteering(40);
							line_des=10;
							LastDeltaDist=DeltaDist;
							DeltaDist=line_des+(AbstandRechts-AbstandLinks);  //deltaDist = e

							dDelta=DeltaDist-LastDeltaDist;
						}
						else if((dRight>9) || (dRight>10 && AbstandFront >170))
						{
							//Driver_SetBack(100);

								Corner=RTurn;  // Rechts
								statecase= Curve;
								Driver_SetSteering(-40);
								line_des=-10;
								LastDeltaDist=DeltaDist;
								DeltaDist=line_des+(AbstandRechts-AbstandLinks);  //deltaDist = e

								dDelta=DeltaDist-LastDeltaDist;
						}

		        }
				if((DeltaDist>3)||(DeltaDist<-3))  //Hysteres f�r mittenController
				{

				    Steer=DeltaDist*kpSteer+dDelta*2.1;  //KD controller
					Driver_SetSteering(Steer);

				}

			break;

			case Curve:

				if(laststate!=statecase)  //Wenn state gewechselt wurde dann UTurn oder normal turn?  und Corner rauschreiben
				{
					if((AbstandFront>=130)&&(Corner==LTurn)&&(AbstandLinks>60)&&(AbstandRechts>15))
					{
						Corner=UTurnL;
						//Driver_LCD_Clear();
						//Driver_LCD_WriteString("UTurnL",6,5,0);
					}
					else if((AbstandFront>=130)&&(Corner==RTurn))
					{
						Corner=UTurnR;
						//Driver_LCD_Clear();
						//Driver_LCD_WriteString("UTurnR",6,5,0);
					}
					else
					{
						//Driver_LCD_Clear();
						//Driver_LCD_WriteString("Corner",6,5,0);
					}
					laststate=statecase;
				}

				if((AbstandLinks<=3) && (Speed<4)  )     //Hindernisserkennung
				{
					statecase=Hinderniss;
				}
				else if ((AbstandRechts<=3) && (Speed<4))
				{
					statecase=Hinderniss;
				}
				else if(AbstandFront<=3)
				{
					statecase=Hinderniss;
				}


			/*	if(AbstandFront>50)
				{
					SpeedDes=60;
				}
				else if(AbstandFront>30)
				{
					SpeedDes=50;
				}
				else
				{
					SpeedDes=40;
				}
*/


				if(Corner==LTurn)
								{
					LCD_BL_OFF;
									Driver_SetSteering(-100);
									if((AbstandFront>AbstandLinks)&&(AbstandFront>110))
									{
										statecase=DriveStraight;
									}
									else if(dRight>15)
									{
										dir=RTurn;
									}
									if(AbstandFront>50)
									{
										SpeedDes=SCornf;
									}
									else if(AbstandFront>30)
									{
										SpeedDes=SCornmid;
									}
									else
									{
										SpeedDes=SCornsl;
									}


								}
				else if(Corner==RTurn)
								{
					LCD_BL_OFF;
									Driver_SetSteering(100);
									if((AbstandFront>AbstandRechts)&&(AbstandFront>110))
									{
										statecase=DriveStraight;
									}
									else if(dLeft>15)
									{dir=LTurn;}

									if(AbstandFront>50)
									{
										SpeedDes=SCornf;
									}
									else if(AbstandFront>30)
									{
										SpeedDes=SCornmid;
									}
									else
									{
										SpeedDes=SCornsl;
									}

								}
					else if(Corner==UTurnR)
								{
						LCD_BL_OFF;
									Driver_SetSteering(80);

									if((AbstandFront>AbstandRechts)&&(AbstandFront>150)&&(AbstandLinks<=70)&&(AbstandRechts<=70))
									{statecase=DriveStraight;}
									else if((AbstandLinks<=60)&&(dLeft>12))
									{

										statecase=DriveStraight;
										//Driver_SetSteering(0);
									}
									else if((AbstandRechts<40)&&(AbstandFront>40)) //abstandfront vorher 110
									{
										//statecase=DriveStraight;
										Corner=LTurn;
										//Driver_SetSteering(0);
									}
									else if((AbstandLinks<=8)&&(AbstandFront>14))
									{
										SpeedDes=70;
										statecase=DriveStraight;
									}

									else if((AbstandLinks<=5)||(AbstandRechts<=3))
									{
										statecase=Hinderniss;
									}
									/*else if((dLeft>10)&&(AbstandFront<=120))
									{
										statecase=DriveStraight;
									}
*/

									if(AbstandFront>50)
									{
										SpeedDes=SUturnf;
									}
									else if(AbstandFront>30)
									{
										SpeedDes=SUturnmid;
									}
									else
									{
										SpeedDes=SUturnsl;
									}

								}

					else if(Corner==UTurnL)
								{
						LCD_BL_ON;
									Driver_SetSteering(-100);

									if((AbstandFront>AbstandLinks)&&(AbstandFront>150)&&(AbstandLinks<=70)&&(AbstandRechts<=70))
									{statecase=DriveStraight;}
									else if((AbstandLinks<25)&&(AbstandFront>80))
									{
										statecase=DriveStraight;

									}
									else if((AbstandRechts<=8)&&(AbstandFront>14))
									{
										SpeedDes=70;
										statecase=DriveStraight;
									}
									else if((AbstandRechts<=5)||(AbstandLinks<=3))
									{
										statecase=Hinderniss;
									}


									if(AbstandFront>50)
									{
										SpeedDes=SUturnf-10;
									}
									else if(AbstandFront>30)
									{
										SpeedDes=SUturnmid-10;
									}
									else
									{
										SpeedDes=SUturnsl-10;
									}

								}



								if(AbstandFront<=5 && (AbstandLinks<5||AbstandRechts<5))
								{
									//Driver_SetBack(50);
									//Driver_SetBrake(100);
									statecase=Hinderniss;

								}
								else if((AbstandLinks<3)||(AbstandRechts<3))
								{
									//Driver_SetBack(50);
									//Driver_SetBrake(100);
									statecase=Hinderniss;
								}

			break;

			//////////////Hinderniss State
			case Hinderniss:
				LCD_BL_OFF;
						//	Driver_LCD_Clear();
							//Driver_LCD_WriteString("Hinderniss",10,5,0);

							if(laststate==DriveStraight)
							{


										if(AbstandFront<=20)
										{
											if(AbstandLinks<AbstandRechts)
											{
												Driver_SetSteering(-100);
												HardBraking=0;

												SpeedDes=SpeedBack;//r�ckw�rts
											}
											else if (AbstandRechts<=AbstandLinks)
											{
												Driver_SetSteering(100);
												HardBraking=0;

												SpeedDes=SpeedBack;
											}
										}
										else
										{
											line_des=0;
											didit=0;
											HardBraking=0;

											if(laststate!=statecase)
											{
												laststate=statecase;
											}
											statecase=DriveStraight;
										}


								}



							else if(laststate==Curve)  ////////Komme aus KURVE
							{
								if(AbstandFront<=30)
								{
									if(AbstandLinks<AbstandRechts)
									{
										Driver_SetSteering(-100);
										HardBraking=0;
										SpeedDes=SpeedBack;
									}
									else if (AbstandRechts<=AbstandLinks)
									{
										Driver_SetSteering(100);
										HardBraking=0;
										SpeedDes=SpeedBack;
									}
								}
								else
									{
									HardBraking=0;
									line_des=0;
									didit=0;

									if(laststate!=statecase)
									{
										laststate=statecase;
									}

									statecase=DriveStraight;

									}
								}


							if(AbstandFront>30)
							{
								HardBraking=0;
								if(laststate!=statecase)
								{
									laststate=statecase;
								}
								statecase=DriveStraight;
							}



			            break;









		}
















		////////////////////////////////////////////////////////////////////

	}
	}







