/*
 * ButtonDriver.h
 *
 *  Created on: 14 Şub 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_BUTTONDRIVER_H_
	#define MODULES_BUTTONDRIVER_H_

	#include <stdint.h>
	extern volatile uint8_t led_state;
	void ButtonDriver_init(void);		// Normal polling için init
	void ButtonDriverInt_init(void);	// Interrupt ile kontrol için init
	int ButtonDriver_get_state(void);


#endif /* MODULES_BUTTONDRIVER_H_ */
