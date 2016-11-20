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
extern int pressed;

int Buttonstate(void)
{
  static int bt_state = 0;
  switch(bt_state)
  {
  case 0:
				if(Buttons.button==1&&Buttons.active==1)
				{
					bt_state = 1;
				}
				else if (Buttons.button==2&&Buttons.active==1)
				{
					bt_state = 2;
				}


  break;
  case 1:{                            //Button 1 wurde gedr�ckt
	  	  	  if(pressed>=20)         //Button 1 wird gehalten
	  	  	  {
	  	  		  bt_state = 3;
	  	  		  lock_hold = 1;

	  	  	  }
	  	  	  else
	  	  	  {

	  	  	  }
  	  	  }
  break;

  case 2:{   							/// Button 2 wurde gedr�ckt
	  	  	  if(pressed>=20)     ///Button 2 wird gehalten
	  	  	  {
	  	  		  bt_state = 4;
	  	  		  lock_hold=1;

	  	  	  }
	  	  	  else
	  	  	  {

	  	  	  }

  	  	  }
  break;
  case 3:						// Button 1 wird gehalten
	  break;
  case 4:						// Button 2 wird gehalten
  	  break;
  }
return bt_state;
}
