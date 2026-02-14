/*
 * LedDriver.c
 *
 *  Created on: 14 Şub 2026
 *      Author: mehmet_dora
 */


#include "stm32f4xx_hal.h"



void LedDriver_init(){

	// AHB1 bus register'larınından 0. bitini set et, GPIOA clock'unu aktif et yani.
	  RCC->AHB1ENR |= 1UL;			// GPIOAEN


	  /*
	  // PA5 OUTPUT
	  GPIOA->MODER &= ~(3UL << (2*5));	// moder bit ikilisini clear et
	  GPIOA->MODER |=  (1UL << (2*5));	// 01 yaparak output ayarla
	  */


	  // Port A ,  Moder5 binary 10 yapılmalı , output moduna almak için.
	  GPIOA->MODER |= (1UL << 2*5);			// 5. pin - High bit 1
	  GPIOA->MODER &= ~(1UL << (2*5 + 1));	// 5. pin - Low  bit 0


	  // PA6 MODER temizle
	  GPIOA->MODER &= ~(3UL << (2*6));

	  // PA6 OUTPUT (01)
	  GPIOA->MODER |=  (1UL << (2*6));

	  // PA6 OUTPUT (01)
	  GPIOA->MODER &= ~(3UL << (2*7));
	  GPIOA->MODER |=  (1UL << (2*7));

}

void LedDriver_on(){
	//GPIOA->ODR |= (1UL << 5);		// 5. biti set etmek
	GPIOA->ODR |= (1UL << 7);
}

void LedDriver_off(){
	//GPIOA->ODR &= ~(1UL << 5);	// 5. biti clear etmek
	GPIOA->ODR &= ~(1UL << 7);
}

