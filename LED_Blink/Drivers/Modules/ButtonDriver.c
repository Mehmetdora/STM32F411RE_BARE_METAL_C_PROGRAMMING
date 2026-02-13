/*
 * ButtonDriver.c
 *
 *  Created on: 13 Şub 2026
 *      Author: mehmet_dora
 */


#include "stm32f4xx_hal.h"


void ButtonDriver_init(){
	// AHB1 bus register'larınından 0. bitini set et, GPIOA clock'unu aktif et yani.
	  RCC->AHB1ENR |= (1UL << 2);	// GPIOCEN

	  // Port C, Moder13 input için 00 yapılmalı, 27 ve 26. bitleri
	  GPIOC->MODER &= ~(1UL << (2*13 + 1));
	  GPIOC->MODER &= ~(1UL << (2*13));
}


int ButtonDriver_get_state(){


	// B1 user butonu pull up direnci ile bağlı olduğundan butona basılınca değeri 0 olur.
	if(!(GPIOC->IDR & (1UL << 13))){	// butona basılmışsa
		return 1;
	}else{
		return 0;						// butona basılmamışsa
	}

}
