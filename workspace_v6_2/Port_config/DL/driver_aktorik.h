
//////////////////Fertig

/*
 * driver_aktorik.h
 *
 *  Created on: 31. Okt. 2016
 *      Author: KlotzSeb
 */

#ifndef DRIVER_DRIVER_AKTORIK_H_
#define DRIVER_DRIVER_AKTORIK_H_

#define StPWM_full_left 2800
#define StPWM_full_right 4350
#define StPWM_middle 3590
#define res_left ((StPWM_middle-StPWM_full_left) / 100)
#define res_right ((StPWM_full_right-StPWM_middle) / 100)

#define MaxRPW 2500
#define MinRPW 3000
#define MaxBRK 3500
#define MinFPW 4000
#define MaxFPW 5000
#define res_throttle ((MaxFPW-MinFPW)/100)
#define res_brk_f ((MinFPW-MaxBRK)/100)
#define res_brk_r ((MaxBRK-MinRPW)/100)
#define res_back ((MinRPW- MaxRPW)/100)

void Driver_SetSteering (int);
void Driver_ESCInit(void);
void Driver_SetThrottle (int);
void Driver_SetBrake (int);
void Driver_SetBack(int);
void Driver_PWMInit(int PWM, int cycle);

#endif /* DRIVER_DRIVER_AKTORIK_H_ */
