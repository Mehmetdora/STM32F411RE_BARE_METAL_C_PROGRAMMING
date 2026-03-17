/*
 * GPIODriver.c
 *
 *  Created on: 16 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include <stdint.h>


void GPIODriver_init(void){
	RCC->AHB1ENR |= (1UL << 0);

	GPIOA->MODER &= (3UL << (2*5));
	GPIOA->MODER |= (1UL << (2*5));

}

void GPIODriver_led_on(void){
	GPIOA->ODR |= (1UL << 5);
}

void GPIODriver_led_off(void){
	GPIOA->ODR &= ~(1UL << 5);
}

void GPIODriver_led_toggle(void){
	GPIOA->ODR ^= (1UL << 5);
}



void GPIODriver_led_toggle_for_1_sn(uint32_t cnt){

	uint8_t a = (1000 / (uint8_t)cnt);
	for(int i = 0; i < cnt ; i++){
		// 1 sn boyunca bu cnt kadar yanıp sönecek
		GPIODriver_led_toggle();
		HAL_Delay(a/2);
	}
}




