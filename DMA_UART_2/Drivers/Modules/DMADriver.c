/*
 * DMADriver.c
 *
 *  Created on: 15 Mar 2026
 *      Author: mehmet_dora
 */


#include "DMADriver.h"
#include "stm32f4xx.h"

char TX_BUFFER[] = "Hello World! Hello World! Hello World! Hello World! Hello World!\r\n";

void DMADriver_init(){



	// DMA CONFIG
	RCC->AHB1ENR |= (1UL << 21);		// DMA1 clock enable

	DMA1_Stream6->CR &= ~(1UL << 0);	// Önce disable et, sonra enable olmasını bekle
	while((DMA1_Stream6->CR & (1UL << 0)) != 0){};

	DMA1_Stream6->CR |= (1UL << 6);	// DMA1 memory to peripheral selected
	DMA1_Stream6->CR |= (1UL << 8);		// DMA1 circular mode enable, 		yani gönderilecek verilerin sonuna gelindiğinde tekrar başa dönüp göndermeye yeniden başlayacak
	DMA1_Stream6->CR &= ~(1UL << 9);	// DMA1 peripheral address sabit , 	sabit çünkü peripheral hep aynı adresten veri okuyor veya alıyor
	DMA1_Stream6->CR |= (1UL << 10);	// DMA1 memory address artsın, 		yani gönderecek olduğumuz array içindeki bir karakteri gönderdikten sonra bu adresi arttırarak bir sonraki karakteri gönderecek
	DMA1_Stream6->CR &= ~(3UL << 11);	// DMA1 peripheral size 8 bit,		UART 8 bit olarak ayarlandığı için 8 bit seçildi
	DMA1_Stream6->CR &= ~(3UL << 13);	// DMA1 memory size 8 bit
	DMA1_Stream6->CR |= (2UL << 16);	// DMA1 priority level high
	DMA1_Stream6->CR |= (4UL << 25);	// DMA1 chanel 4 selected

	DMA1_Stream6->NDTR = sizeof(TX_BUFFER) - 1;				// kaç kere veri gönderileceğini belirtir , buffer boyutu kadar olmalı
	DMA1_Stream6->PAR = (uint32_t)&USART2->DR;		// Peripheral adress
	DMA1_Stream6->M0AR = (uint32_t)&TX_BUFFER;		// Memory adress

	DMA1_Stream6->FCR |= (2UL << 0);	// 3/4 treshold FIFO






}

void DMADriver_start(){
	DMA1_Stream6->CR |= (1UL << 0);	// DMA2 stream enable
}











