/*
 * ADCDriver.c
 *
 *  Created on: 27 Şub 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>


void ADCDriver_init(void){




	RCC->APB2ENR |= (1UL << 8);		// ADC için clock enable


	RCC->AHB1ENR |= (1UL << 0);		// GPIOA clock enable, PA0 pini ile analog sinyali ölçeceğiz


	GPIOA->MODER |= (3UL << 0);		// PA0 için analog mod enable , sinyalin analog olacağını bildiriyoruz


	ADC1->CR2 = 0UL;			// ADC1 ayarlarını resetleme
	ADC1->CR2 |= (1UL << 0);	// ADON biti(on/off) ile ADC aktifleştirme
	while((ADC1->CR2 & (1UL)) == 0){};	// ON olana kadar bekle, kesin şekilde aktif edilsin

	ADC1->CR1 &= ~(3UL << 24);	// Hangi aralıktaki sayılar arasında değerlendirileceği(resolution)

	ADC1->SQR3 |= 0UL;			// 0. kanalın seçilmesi

	ADC1->CR2 |= (1UL << 0);	// Tekrar ADC1 aktif edilmesi





}







