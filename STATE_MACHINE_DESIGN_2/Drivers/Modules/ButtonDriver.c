/*
 * ButtonDriver.c
 *
 *  Created on: 9 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include <stdint.h>



void ButtonDriver_init(void){

	RCC->AHB1ENR |= (1UL << 2);

	GPIOC->MODER &= ~(3UL << (2 * 13));


	GPIOC->PUPDR &= ~(3UL << (2 * 13));
	GPIOC->PUPDR |= (1UL << (2 * 13));




}
uint8_t ButtonDriver_read(void){


	return (GPIOC->IDR & (1UL << 13)) ? 0 : 1;



}


