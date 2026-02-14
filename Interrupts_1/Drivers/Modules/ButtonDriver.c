/*
 * ButtonDriver.c
 *
 *  Created on: 14 Şub 2026
 *      Author: mehmet_dora
 */



#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "ButtonDriver.h"




void ButtonDriverInt_init(){

	// HAL ile RCC üzerinden GPIOC'nin clock'unu aktif et
	// GPIOC AHB1 bus üzerindedi ve clock enable yapılmadan kullanılamaz
	// __HAL_RCC_GPIOC_CLK_ENABLE();	// HAL ile clock açma
	RCC->AHB1ENR |= (1UL << 2);			// Register ile clock açma


	RCC->AHB1ENR |= 1UL;	// GPIOA'nın clock'unu aktif et

	// HAL ile RCC üzerinden SYSCFG-system config'in clock'unu aktif et- EXTI(external interrupt ) seçimi için
	__HAL_RCC_SYSCFG_CLK_ENABLE();


	// mode: input(00)
	GPIOC->MODER &= ~(3UL << (2*13));	// 13. pinini input yap
	GPIOA->MODER &= ~(3UL << (2*4));	// 4. pinini input yap

	// pull up/down:
	GPIOC->PUPDR &= ~(3UL << 26);
	GPIOC->PUPDR |=  (1UL << 26);  // 13. pinini Pull-up yap

	GPIOC->PUPDR &= ~(3UL << (2*4));
	GPIOC->PUPDR |=  (1UL << (2*4));  // 4. pinini Pull-up yap


	// EXTI13 için GPIOC seç
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	// yada SYSCFG->EXTICR[3] |= 0x20;


	// 2. register ile 4-7 pinlerinin EXTI seçimlerini yap, Port A için
	SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PA;


	// Unmask haline getirme
	EXTI->IMR |= EXTI_IMR_IM13;
	EXTI->IMR |= EXTI_IMR_IM4;


	// falling edge interrupt
	EXTI->FTSR |= EXTI_FTSR_TR13;
	EXTI->FTSR |= EXTI_FTSR_TR4;


	NVIC_SetPriority(EXTI15_10_IRQn,1);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	NVIC_SetPriority(EXTI4_IRQn,1);
	NVIC_EnableIRQ(EXTI4_IRQn);

}

void EXTI15_10_IRQHandler(void){	// bu özel ISR fonk isimleri startup_stm32.s dosyasında bulunur

	if(EXTI->PR & EXTI_PR_PR13){

		// reset EXTI->PR
		EXTI->PR |= EXTI_PR_PR13;

		// button toggling
		GPIOA->ODR ^= GPIO_PIN_5;
		GPIOA->ODR ^= GPIO_PIN_6;

	}

}


void EXTI4_IRQHandler(void){	// PA4 external için tetiklenecek interrupt ile çalışması istenilen özel kod

	if(EXTI->PR & EXTI_PR_PR4){

		EXTI->PR |= EXTI_PR_PR4;


		led_state ^= 1;
	}

}


// Interrupt kullanımı olmayan init() kodları
void ButtonDriver_init(){
	// AHB1 bus register'larınından 3. bitini set et, GPIOC clock'unu aktif et yani.
	RCC->AHB1ENR |= (1UL << 2);	// GPIOCEN
	RCC->AHB1ENR |= 1UL;		// GPIOAEN

	// Port C, Moder13 input için 00 yapılmalı, 27 ve 26. bitleri
	GPIOC->MODER &= ~(1UL << (2*13 + 1));
	GPIOC->MODER &= ~(1UL << (2*13));

	// yine aynı şekilde input yapmak için 8-9. bitleri 0 yap
	GPIOA->MODER &= ~(3UL << (2*4));	// 4. pinini input yap

	// 4. pinini Pull-up yap
	GPIOA->PUPDR &= ~(3UL << (2*4));
	GPIOA->PUPDR |=  (1UL << (2*4));

}



// Buton kontrolü
int ButtonDriver_get_state(){

	// B1 user butonu pull up direnci ile bağlı olduğundan butona basılınca değeri 0 olur.
	if(!(GPIOC->IDR & (1UL << 13))){	// butona basılmışsa
		return 1;
	}else{
		return 0;						// butona basılmamışsa
	}

}
