/*
 * LedDriver.c
 *
 *  Created on: 13 Şub 2026
 *      Author: mehmet_dora
 */

#include "stm32f4xx_hal.h"



void LedDriver_init(){

	// AHB1 bus register'larınından 0. bitini set et, GPIOA clock'unu aktif et yani.
	  RCC->AHB1ENR |= 1UL;			// GPIOAEN

	  // Port A ,  Moder5 binary 10 yapılmalı , output moduna almak için.
	  GPIOA->MODER |= (1UL << 2*5);			// 5. pin - High bit 1
	  GPIOA->MODER &= ~(1UL << (2*5 + 1));	// 5. pin - Low  bit 0

}

void LedDriver_on(){
	GPIOA->ODR |= (1UL << 5);		// 5. biti set etmek
}

void LedDriver_off(){
	GPIOA->ODR &= ~(1UL << 5);	// 5. biti clear etmek
}
