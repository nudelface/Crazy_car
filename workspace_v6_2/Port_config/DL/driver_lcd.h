/*
 * driver_lcd.h
 *
 *  Created on: 14.11.2016
 *      Author: Sebo
 */

#ifndef DL_DRIVER_LCD_H_
#define DL_DRIVER_LCD_H_

#define LCD_RESET_c 0xD2
#define LCD_BIAS 0xA3
#define ADC_SEL_NORMAL 0xA0
#define COMMON_REVERSE 0xC8
#define RES_RATIO 0x24
#define ELEC_VOL_MODE 0x81
#define ELEC_VOL_VALUE 0x0F
#define POWER_CONT 0x2F
#define DISPLAY_ON 0xAF
#define DISPLAY_line_start 0x40
#define DISPLAY_col_msb 0x10
#define DISPLAY_col_lsb 0x0
#define DISPLAY_Page 0xB0
#define DISPLAY_ALL 0xA5
#define CHAR_WIDTH 6


void Driver_LCD_WriteCommand(unsigned char *, unsigned char);
void Driver_LCD_Init(void);
void Driver_LCD_Clear(void);
void Driver_LCD_Clearpage(unsigned char);
void Driver_LCD_Clearspace(unsigned char, unsigned char, unsigned char);
void Driver_LCD_SetPosition(unsigned char, unsigned char);
void Driver_LCD_WriteString(unsigned char *, unsigned char, unsigned char, unsigned char);
void Driver_LCD_WriteUInt( int, unsigned char, unsigned char);
void itoa(unsigned int , char* , unsigned char);


#define command (P8OUT &= ~LCD_DATACMD)
#define LCD_data (P8OUT |= LCD_DATACMD)

#define LCD_RESET_L (P9OUT &= ~LCD_RESET)
#define LCD_RESET_H (P9OUT |= LCD_RESET)





#endif /* DL_DRIVER_LCD_H_ */