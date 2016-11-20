/*
 * hal_button.c
 *
 *  Created on: 15.11.2016
 *      Author: Sebo
 */

#include "hal_button.h"
#include "Interrupts.h"
#include "hal_timerB0.h"

extern int timebase;
extern ButtonCom Buttons;
extern int lock_hold;


int Buttonstate(void)
{
  static int bt_state = 0;
  switch(bt_state)
  {
  case 0: {
				if(Buttons.button==1&&Buttons.active==1)
				{
					bt_state = 1;
				}
				else if (Buttons.button==2&&Buttons.active==1)
				{
					bt_state = 2;
				}

  	  	  }
  break;
  case 1:{
	  	  	  if(pressed>=20)
	  	  	  {
	  	  		  bt_state = 3;
	  	  		  lock_hold = 1;
	  	  	  }
	  	  	  return 1;
  	  	  }
  break;
  case 2:
  break;
  case 3:
	  break;

  }

}
