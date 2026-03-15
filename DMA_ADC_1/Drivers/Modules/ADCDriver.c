/*
 * ADCDriver.c
 *
 *  Created on: 14 Mar 2026
 *      Author: mehmet_dora
 */
#include <stm32f4xx.h>



void ADCDriver_init(){
	// GPIO CONFIG
	RCC->AHB1ENR |= (1UL << 0);		// GPIOA clock enable

	GPIOA->MODER |= (3UL << 0);		// PA0 analog mode
	GPIOA->OSPEEDR |= (3UL << 0);	// PA0 high speed




	// ADC CONFIG
	RCC->APB2ENR |= (1UL << 8);		// ADC1 clock enable

	ADC1->CR1 &= ~(1UL << 8);		// tek kanal olduğundan , scan mode disable
	ADC1->CR1 &= ~(3UL << 24);		// resolution -> 12 bit
	ADC1->CR2 |= (1UL << 0);		// ADC1 enable
	ADC1->CR2 |= (1UL << 1);		// continuous conversion mode enable
	ADC1->CR2 |= (1UL << 8);		// ADC1 DMA mode enable
	ADC1->CR2 |= (1UL << 9);		// ADC1 DMA enable
	ADC1->CR2 |= (1UL << 10);		// ADC1 EOC enable

	ADC1->SQR1 &= ~(15UL << 20);	// Tek conversion bildiriliyor
	ADC1->SQR3 &= ~(15UL << 0);		// Sequence olarak 0. olarak başlasın

}
