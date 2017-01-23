/*
 * Interrupts.h
 *
 *  Created on: 17.10.2016
 *      Author: Sebastian
 */

#ifndef HAL_INTERRUPTS_H_
#define HAL_INTERRUPTS_H_


#define START_flag (P1IFG & START_Button)
#define STOP_flag (P1IFG & STOP_Button)
#define DIR (P1IN&RPM_Sensor_DIR)
#define Timer1 (TBCCTL0&CCIFG)
#define OUT1 (TBCCTL0&OUT)


typedef struct{
	unsigned char active; //TRUE 1 /False 0
	unsigned char button; //Button number
}ButtonCom;



#endif /* HAL_INTERRUPTS_H_ */
