/*
 * LedDriver.c
 *
 *  Created on: 18 Şub 2026
 *      Author: mehmet_dora
 */

#include <stdint.h>
#include "stm32f4xx_hal.h"




void LedDriver_init(void){


	RCC->AHB1ENR |= (1UL << 0);	// GPIOA clock enable

	GPIOA->MODER &= ~(3UL << (2*5));	// MODER clear
	GPIOA->MODER |= (1UL << (2*5));


}
