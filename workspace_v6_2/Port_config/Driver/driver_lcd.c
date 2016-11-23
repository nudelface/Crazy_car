/*
 * driver_lcd.c
 *
 *  Created on: 14.11.2016
 *      Author: Sebo
 */
#include "driver_lcd.h"
#include "HAL\hal_gpio.h"
#include "HAL\hal_usciB1.h"


extern USCIB1_SPICom SpiCom;
extern unsigned char cmdArr[];

unsigned char InitArr[9] = {LCD_RESET, LCD_BIAS, ADC_SEL_NORMAL, COMMON_REVERSE, RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, POWER_CONT, DISPLAY_ON};

void Driver_LCD_WriteCommand(unsigned char *data, unsigned char len)
{
	int i = 0;
	command;

	for(i = 0; i <= len; i++)
	{
		SpiCom.TxData.Data[i] = *data;
		data++;
	}

	SpiCom.TxData.len = len;
	if(SpiCom.Status.B.TXSuc==1)
	{
		HAL_USCIB1_Transmit();
	}
}

void Driver_LCD_Init(void)
{
	LCD_RESET_L;
	__delay_cycles(100000);
	LCD_RESET_H;
	Driver_LCD_WriteCommand(&InitArr[0], 9);

	__delay_cycles(100000);

	//LCD Clear
}

void Driver_LCD_Clear(void)
{
	unsigned char column = 0;
	unsigned char column_Lsb;
	unsigned char column_Msb;
	if(column < 16)
	{
		column_Lsb = column;
		column_Msb = 0;
	}
	else
	{
		column_Msb = column/15;
		column_Lsb =

	}
	unsigned char ClearArr[2] = {16+column/15}


}

void Driver_LCD_SetPosition(unsigned char page, unsigned char col)
{

}
