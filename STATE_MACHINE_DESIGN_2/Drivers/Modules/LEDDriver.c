/*
 * LEDDriver.c
 *
 *  Created on: 9 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>


void LEDDriver_init(void){

	RCC->AHB1ENR &= ~(1UL << 0);
	RCC->AHB1ENR |= (1UL << 0);


	GPIOA->MODER &= ~(3UL << (2 * 5));
	GPIOA->MODER |= (1UL << (2 * 5));




}
void LEDDriver_on(void){

	GPIOA->ODR |= (1UL << 5);

}
void LEDDriver_off(void){

	GPIOA->ODR &= ~(1UL << 5);

}


void LEDDriver_blink(uint16_t delay_ms){
	GPIOA->ODR &= ~(1UL << 5);
	HAL_Delay(delay_ms);
	GPIOA->ODR |= (1UL << 5);
}


void LEDDriver_toggle(void){

	GPIOA->ODR ^= (1UL << 5);

}
