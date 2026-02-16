/*
 * LedDriver.c
 *
 *  Created on: 15 Şub 2026
 *      Author: mehmet_dora
 */




#include "stm32f4xx_hal.h"


void LedDriver_init(void){

	RCC->AHB1ENR |= (1UL << 0);

	GPIOA->MODER &= ~(3UL << (2*5));
	GPIOA->MODER |= (1UL << (2*5));


}


void LedDriver_on(void){
	GPIOA->ODR |= (1UL << (2*5));
}

void LedDriver_off(void){
	GPIOA->ODR &= ~(1UL << (2*5));
}

void LedDriver_toggle(void){
	GPIOA->ODR ^= (1UL << 5);
}
