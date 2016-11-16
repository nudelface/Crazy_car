/*
 * hal_usc.h
 *
 *  Created on: 25. Okt. 2016
 *      Author: Niki
 */

#ifndef HAL_HAL_USC_H_
#define HAL_HAL_USC_H_

#include <msp430.h>

#define XTAL_FREQU 20000000  //20MHz
#define MCLK_FREQU 20000000 //20MHz
#define SMCLK_FREQU 2500000 //2.5MHz


void HAL_UCS_Init(void);


#endif /* HAL_HAL_USC_H_ */
