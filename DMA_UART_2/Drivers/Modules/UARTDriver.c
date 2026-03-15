/*
 * UARTDriver.c
 *
 *  Created on: 15 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>



// UART CONFIG


void UARTDriver_init(){


	// GPIO CONFIG
	RCC->AHB1ENR |= (1UL << 0);		// GPIOA clock enable

	// USART2 , AF olarak PA2 ve PA3 kullanıyor, bu pinler buna göre ayarlanmalı
	GPIOA->MODER |= (2UL << (2*2));		// PA2 tx AF mode
	GPIOA->MODER |= (2UL << (2*3));		// PA3 rx AF mode

	GPIOA->OSPEEDR |= (3UL << (2*2));	// PA2 high speed
	GPIOA->OSPEEDR |= (3UL << (2*3));	// PA3 high speed

	// PA2 ve PA3 seçildiği için AF tablosundaki AFR2 ve AFR3 AF7 olarak ayarlanmalı
	GPIOA->AFR[0] |= (7UL << (4*2)) | (7UL << (4*3));		// PA2 ve PA3,  AF7 seçildi




	RCC->APB1ENR |= (1UL << 17);

	USART2->CR1 &= ~(1UL << 12);	// 8 bit data transfer
	USART2->CR1 &= ~(1UL << 10);	// parity control bit disable

	USART2->BRR = (22UL << 4) | (13UL);	// 115200 boud rate

	USART2->CR3 |= (1UL << 7);		// DMA enable for uart tx

	USART2->CR1 |= (1UL << 3);		// USART TX enable
	USART2->CR1 |= (1UL << 13);		// USART2 start
}
