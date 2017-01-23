/*
 * hal_adc12.h
 *
 *  Created on: 29.11.2016
 *      Author: Sebo
 */

#ifndef HAL_HAL_ADC12_H_
#define HAL_HAL_ADC12_H_


void HAL_Adc_init(void);

typedef struct {
	union {
		unsigned char R;
		struct {
			unsigned char ADCrdy:1;
			unsigned char dummy:7;
		}B;
	}Status;
	unsigned int Bit_right;
	unsigned int Bit_left;
	unsigned int Bit_front;
	unsigned int vBat;

}ADC12Com;

#define Res 3,739 / 4095   //3,739 V geteilt durch die Auflösung des ADC 12-Bit


#endif /* HAL_HAL_ADC12_H_ */
