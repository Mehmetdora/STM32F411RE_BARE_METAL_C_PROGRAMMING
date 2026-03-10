/*
 * LEDDriver.h
 *
 *  Created on: 9 Mar 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_LEDDRIVER_H_
#define MODULES_LEDDRIVER_H_


#include <stdint.h>

void LEDDriver_init(void);
void LEDDriver_on(void);
void LEDDriver_off(void);
void LEDDriver_blink(uint16_t delay_ms);


void LEDDriver_toggle(void);





#endif /* MODULES_LEDDRIVER_H_ */
