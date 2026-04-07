/*
 * TimerDriver.h
 *
 *  Created on: Apr 5, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_TIMERDRIVER_H_
#define MODULES_TIMERDRIVER_H_

#include "stdint.h"


extern volatile uint32_t counter;

void TimerDriver_init(void);
void TimerDriver_enable(void);
void TimerDriver_disable(void);
void TimerDriver_set_period(uint32_t period);


#endif /* MODULES_TIMERDRIVER_H_ */
