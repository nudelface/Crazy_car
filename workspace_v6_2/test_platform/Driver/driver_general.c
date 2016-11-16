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

	Driver_SetSteering(50);  // -100(link) bis +100(rechts) kann übergeben werden
    Driver_ESCInit();
}
