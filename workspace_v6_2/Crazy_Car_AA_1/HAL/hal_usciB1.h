/*
 * hal_usciB1.h
 *
 *  Created on: 07.11.2016
 *      Author: Sebo
 */

#ifndef HAL_HAL_USCIB1_H_
#define HAL_HAL_USCIB1_H_

#define SCLK_diver 2500000/100000

void HAL_USCIB1_Init(void);
void HAL_USCIB1_Transmit (void);

typedef struct {
	union{
			unsigned char R;
			struct {
					unsigned char TXSuc:1;  //Bit=1 wenn Daten  übertragen wurden
					unsigned char dummy:7;
			  }B;
		  }Status;
   struct{
			  unsigned char len;  //Länge der Daten in Bytes die übertragen werden
			  unsigned char cnt;  //Index auf Momentan übertragene Daten
			  unsigned char Data[256]; //TX Daten Array
		  }TxData;
   struct{
			  unsigned char len;   //Länge der empfangenen Daten
			  unsigned char Data[256];  //Rx Daten Array
		  }RXData;
}USCIB1_SPICom;


#endif /* HAL_HAL_USCIB1_H_ */
