/*
 * driver_general.c
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */
#include <DL/driver_aktorik.h>
#include <DL/driver_lcd.h>
#include <msp430.h>
#include "..\HAL\hal_timerA1.h"
#include "..\HAL\hal_usc.h"




void Driver_Init(void)
{
	Driver_LCD_Init();      //LCD Initialisieren
	Driver_SetSteering(0);  //Steering initialisieren
    Driver_ESCInit();       // ESC Initialisieren
}
