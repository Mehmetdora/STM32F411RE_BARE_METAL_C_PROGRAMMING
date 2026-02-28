/*
 * GPIODriver.c
 *
 *  Created on: 28 Şub 2026
 *      Author: mehmet_dora
 */

#include <stm32f4xx.h>

void GPIODriver_init(void){

	RCC->AHB1ENR |= (1UL << 0);


	GPIOA->MODER &= ~(3UL << (2*5));
	GPIOA->MODER |= (1UL << (2*5));


}
