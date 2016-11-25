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

unsigned char InitArr[10] = {LCD_RESET_c, ADC_SEL_NORMAL, LCD_BIAS, COMMON_REVERSE,DISPLAY_line_start, POWER_CONT, RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, DISPLAY_ON};
unsigned char Init2Arr[9]={ LCD_RESET, LCD_BIAS, ADC_SEL_NORMAL, COMMON_REVERSE, RES_RATIO, ELEC_VOL_MODE, ELEC_VOL_VALUE, POWER_CONT, DISPLAY_ON };

void Driver_LCD_WriteCommand(unsigned char *data, unsigned char len)
{
	P8OUT&=~LCD_SPI_CS;
	unsigned int i = 0;
	command;
	SpiCom.TxData.Data[0] = 0;
	for(i = 0; i < len; i++)
	{
		SpiCom.TxData.Data[i+1] = *data;
		data++;
	}

	SpiCom.TxData.len = len+1;
	if(SpiCom.Status.B.TXSuc==1)
	{
		HAL_USCIB1_Transmit();

	}
	P8OUT|=LCD_SPI_CS;
	LCD_data;
}

void Driver_LCD_Init(void)
{
	LCD_RESET_L;
	__delay_cycles(100000);
	LCD_RESET_H;
	Driver_LCD_WriteCommand(&Init2Arr[0], 9);

	__delay_cycles(100000);

	Driver_LCD_Clear();

}

void Driver_LCD_Clear(void)  //test Sebo
{

	LCD_data;
	int c_p=0;
	unsigned char column = 0;
	int column_Msb = column&0xF0;
	int column_Lsb = column&0x0F;
	unsigned char clearArr[5] = {DISPLAY_line_start,DISPLAY_Page,DISPLAY_col_msb,DISPLAY_col_lsb};

	Driver_LCD_WriteCommand(&clearArr[0],4);
	LCD_data;
	P8OUT&=~LCD_SPI_CS;
	SpiCom.TxData.len=0x83;
	HAL_USCIB1_Transmit();
	P8OUT|=LCD_SPI_CS;
	while (c_p <= 7)
	{
			if(SpiCom.Status.B.TXSuc==1)
			{
				c_p++;
				clearArr[1]=DISPLAY_Page+c_p;
				Driver_LCD_WriteCommand(&clearArr[1],3);
				P8OUT&=~LCD_SPI_CS;
				LCD_data;
				SpiCom.TxData.Data[3]=0xFF;
				SpiCom.TxData.Data[4]=0x0F;
				SpiCom.TxData.Data[5]=0x00;
				SpiCom.TxData.len=0x83;
				HAL_USCIB1_Transmit();
				P8OUT|=LCD_SPI_CS;
			}

	}

	Driver_LCD_WriteCommand(&clearArr[4],1);
	P8OUT|=LCD_SPI_CS;
}

void Driver_LCD_SetPosition(unsigned char page, unsigned char col)
{

}
