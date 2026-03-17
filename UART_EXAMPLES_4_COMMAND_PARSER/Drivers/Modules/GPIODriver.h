/*
 * GPIODriver.h
 *
 *  Created on: 16 Mar 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_GPIODRIVER_H_
#define MODULES_GPIODRIVER_H_

#include <stdint.h>
void GPIODriver_init(void);
void GPIODriver_led_on(void);
void GPIODriver_led_off(void);
void GPIODriver_led_toggle(void);
void GPIODriver_led_toggle_for_1_sn(uint32_t cnt);

#endif /* MODULES_GPIODRIVER_H_ */
