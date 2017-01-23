/*
 * hal_adc12.c
 *
 *  Created on: 29.11.2016
 *      Author: Sebo
 */


#include "hal_adc12.h"

#include <msp430.h>

ADC12Com ADC1;

void HAL_Adc_init(void)    //ADC Initialisieren
{
	//Einlesen der Sensorwerte  Abstandssensoren

	ADC12MCTL0 |= ADC12INCH_0+ADC12SREF_4;      //Input Channel A0    //sRef4= VR+=AVCC  VR-=VREF-/VeRef-       //Dist Right
	ADC12MCTL1 |= ADC12INCH_1+ADC12SREF_4;      //Input Channel A1    //sRef4= VR+=AVCC  VR-=VREF-/VeRef-       //Dist Left
	ADC12MCTL2 |= ADC12INCH_2+ADC12SREF_4;      //Input Channel A2    //sRef4= VR+=AVCC  VR-=VREF-/VeRef-        //Dist Front
	ADC12MCTL3 |= ADC12INCH_3+ADC12EOS+ADC12SREF_4;  //Input Channel A3    //sRef4= VR+=AVCC  VR-=VREF-/VeRef-  //EOS-End of Sequence        //VBAtt
	//ADC12IE |= ADC12IE3;
	ADC12CTL2 |= ADC12PDIV+ADC12RES_2;          //PDIV= Predivider von 4 (clock)    //ADC12RES_2  Auflösung 12Bit (13 Clock cycles)
	ADC12CTL1 |= ADC12SHP+ADC12DIV_7+ADC12SHS_2+ADC12CONSEQ_1;    //SHP = SampCon Signal kommt von Sampletime  DIV_7= Divider bei 8 (clock)   SHS=TimerSource timerB   Conseq_1=Sequence of Channels
	ADC12CTL0 |= ADC12MSC+ADC12REF2_5V+ADC12REFON+ADC12ON+ADC12ENC+ADC12SHT0_11;     //MSC=Rising edge triggert samplingtimer  Ref2_5V=2,5V Refspannung  REFON= Reference ON  ADC12On=ADC ON  ADC12ENC=ADC12 Enabble  SHT0_11=768 ADC12 Cycles = Sample and Hold Time


	// Aktuelle AUflösung ist: 0,000913V / Bit
}

