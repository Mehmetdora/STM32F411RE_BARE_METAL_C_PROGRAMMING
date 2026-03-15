/*
 * DMADriver.c
 *
 *  Created on: 14 Mar 2026
 *      Author: mehmet_dora
 */


#include "DMADriver.h"
#include "stm32f4xx.h"

uint16_t ADC_DATA[1];

void DMADriver_init(){


	// DMA CONFIG
	RCC->AHB1ENR |= (1UL << 22);		// DMA2 clock enable


	DMA2_Stream0->CR &= ~(1UL << 0);	// Önce disable et, sonra enable olmasını bekle
	while((DMA2_Stream0->CR & (1UL << 0)) != 0){};

	DMA2_Stream0->CR &= ~(3UL << 6);	// DMA2 peripheral to memory selected
	DMA2_Stream0->CR |= (1UL << 8);		// DMA2 circular mode enable
	DMA2_Stream0->CR &= ~(1UL << 9);	// DMA2 peripheral address sabit
	DMA2_Stream0->CR |= (1UL << 10);	// DMA2 memory address artsın
	DMA2_Stream0->CR |= (1UL << 11);	// DMA2 peripheral size 16 bit	-> ADC nin DR registerından okunabilecek max veri 16 bitlik
	DMA2_Stream0->CR |= (1UL << 13);	// DMA2 memory size 16 bit
	DMA2_Stream0->CR |= (2UL << 16);	// DMA2 priority level high
	DMA2_Stream0->CR &= ~(7UL << 25);	// DMA2 chanel 0 selected

	DMA2_Stream0->NDTR = 1;	// Kaç kere transfer yapılacağı, bir tane adc okunacak
	DMA2_Stream0->PAR = (uint32_t)&ADC1->DR;	// Peripheral adress
	DMA2_Stream0->M0AR = (uint32_t)&ADC_DATA;	// Memory adress

	DMA2_Stream0->FCR |= (2UL << 0);	// 3/4 treshold FIFO



	ADC1->CR2 |= (1UL << 30);		// ADC1 start conversion
	DMA2_Stream0->CR |= (1UL << 0);	// DMA2 stream enable

}
