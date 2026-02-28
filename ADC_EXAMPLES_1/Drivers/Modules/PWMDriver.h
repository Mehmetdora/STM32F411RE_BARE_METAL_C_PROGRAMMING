/*
 * PWMDriver.h
 *
 *  Created on: 20 Şub 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_PWMDRIVER_H_
#define MODULES_PWMDRIVER_H_


#include <stdint.h>
void PWMDriver_init(void);
//void PWMDriver_set_duty(PWM_HANDLE_T *pwm ,  uint32_t duty);
void PWMDriver_on(void);
void PWMDriver_off(void);
void PWMDriver_set_periot(uint32_t val);


#endif /* MODULES_PWMDRIVER_H_ */
