/*
 * driver_general.c
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */
#include <msp430.h>
#include "..\HAL\hal_timerA1.h"
#include "..\HAL\hal_usc.h"
#include "driver_aktorik.h"



void Driver_Init(void)
{

	Driver_SetSteering(0);  // -560 bis 560 kann �bergeben werden
    Driver_ESCInit();
}
