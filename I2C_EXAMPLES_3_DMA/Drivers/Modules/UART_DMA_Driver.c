/*
 * UART_DMA_Driver.c
 *
 *  Created on: Apr 7, 2026
 *      Author: mehmet_dora
 */

#include "stm32f4xx.h"
#include "I2C_DMA_Driver.h"
#include "UART_DMA_Driver.h"

#include "string.h"
#include "stdint.h"


#define PACKET_DATA_SIZE  (30 * 12)   				// 360 byte
#define PACKET_TOTAL_SIZE (PACKET_DATA_SIZE + 5) 	// header+len+checksum

static uint8_t tx_packet[PACKET_TOTAL_SIZE];







void UART_init(void){

	RCC->APB1ENR |= (1UL << 17); 		// usart2 clock enable, AF7 için


	RCC->AHB1ENR |= (1UL << 0);			// A portunun clock enable
	// USART2 çıkışları A portundan sağlanacağı için bu port çıkışlarının clockunu enable et



	// GPIOA pinlerinin modunu AF olarak ayarlanması , PA2 ve PA3
	GPIOA->MODER &= ~(3UL << (2*2));
	GPIOA->MODER &= ~(3UL << (2*3));

	GPIOA->MODER |= (2UL << (2*2));
	GPIOA->MODER |= (2UL << (2*3));



	// haberleşme için ilgili pinlerin edge değişimlerinin high speed olarak ayarlanması , PA2 ve PA3
	GPIOA->OSPEEDR |= (3UL << (2*2)) | (3UL << (2*3));





	// İlgili pinlerin alternate fonksiyon olarak ayarlanması

	GPIOA->AFR[0] |= (7UL << (4*2)) | (7UL << (4*3));


	USART2->CR1 &= ~(1UL << 12);	// Data boyutunun 8 bit olarak ayarlanması


	// BAUD RATE Hesaplama
	/*
	 * Bu BBR register'ına yazılacak değer ,APB1 Clock frekansı/(Oversampling x USARTDIV) = BAUD RATE olarak hesaplanır.
	 * Elimizde Clock frekansı ve istesiğimiz Baud rate değeri ve istenen oversampling değerleri ile
	 * Usartdiv değeri bulunur. Bu değer kesirli çıkabilir. Hatayı en aza indirmek için çıkan sonucun
	 * tam kısmı 15-4 bitlerine yazılır. Kalan ondalık kısmı ise 0-3 bitlerine yazılır.
	 *
	 * Buradaki sonucun kesirli kısmı önce oversampling değeri ile çarpılır, ardından çıkan sonuç
	 * en yakın tam sayıya yuvarlandıktan sonra bu tam sayı hex formatında 0-3 bitlik fraction kısmına yazılır.
	 *
	 */
	/*
	 * Clock sinyali  = 42.000.000
	 * istenen Baud rate = 115200
	 * oversampling = 16
	 * Sonuç olarak bulunan USARTDIV = 22.786(22 -> mantissa, 0.786 -> fraction)
	 * Mantissa = 22
	 * Fraction = 12.576'dan -> 13 yani hex D
	 * BRR = (Mantissa << 4) | Fraction
	 */

	USART2->BRR = (22UL << 4) | (13UL);		// eski değerlerlerin kalmaması için direkt = kullan


	/*

	// Transmitter enable ve receiver enable yapmak

	USART2->CR1 |= (1UL << 2) | (1UL << 3);


	// USART2 RX interrupt enable, veri almaya hemen başlanabilir
	USART2->CR1 |= (1UL << 5);		// Bu bit RXENIE ile RX için interruot a izin verir
	NVIC_EnableIRQ(USART2_IRQn);	// Bu NVIC in bu interrupt için erişilebilmesini sağlar.
	*/


	USART2->CR1 |= (1UL << 3);		// TX enable
	USART2->CR3 |= (1UL << 7);		// DMA TX mode enable
	USART2->CR1 |= (1UL << 13);		// Usart2 enable


}



void UART_tx_DMA_stream0_init(void){


	// stream 6 , channel 4 -> usart2 tx


	RCC->AHB1ENR |= (1UL << 21);		// DMA1 clock enable

	DMA1_Stream6->CR &= ~(1UL << 0);	// Önce disable et, sonra enable olmasını bekle
	while((DMA1_Stream6->CR & (1UL << 0)) != 0){};

	DMA1_Stream6->CR |= (4UL << 25);	// chanel 4 selected
	DMA1_Stream6->CR |= (1UL << 10);	// memory increment enable
	DMA1_Stream6->CR |= (1UL << 6);		// memory to peripheral


	DMA1_Stream6->CR &= ~(1UL << 9);	// peripheral address sabit
	DMA1_Stream6->CR &= ~(3UL << 11);	// peripheral size 8 bit,
	DMA1_Stream6->CR &= ~(3UL << 13);	// DMA1 memory size 8 bit
	DMA1_Stream6->CR |= (2UL << 16);	// DMA1 priority level high

	DMA1_Stream6->CR |= (1UL << 4);		// Transfer complete interrupt enable
	DMA1_Stream6->FCR &= ~(1UL << 2);	// direct mode enable



	DMA1_Stream6->PAR = (uint32_t)&USART2->DR;		// Peripheral adress
	//DMA1_Stream6->M0AR = (uint32_t)CIRC_BUFFER;		// Memory adress

	NVIC_SetPriority(DMA1_Stream6_IRQn, 1);
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);

}




void UART2_send_packet(volatile uint8_t* CIRC_BUFFER) {
    // Header
    tx_packet[0] = 0xAA;
    tx_packet[1] = 0xBB;

    // Length
    tx_packet[2] = (PACKET_DATA_SIZE >> 8) & 0xFF;
    tx_packet[3] =  PACKET_DATA_SIZE & 0xFF;

    // Data: CIRC_BUFFER içeriğini byte byte yaz
    // (zaten uint8_t buffer kullanıyorsan direkt memcpy)
    memcpy(&tx_packet[4], (void*)CIRC_BUFFER, PACKET_DATA_SIZE);

    // Checksum (XOR)
    uint8_t crc = 0;
    for (uint16_t i = 4; i < 4 + PACKET_DATA_SIZE; i++) {
        crc ^= tx_packet[i];
    }
    tx_packet[4 + PACKET_DATA_SIZE] = crc;

    // DMA başlat
    DMA1_Stream6->CR  &= ~(1UL << 0);
    while (DMA1_Stream6->CR & (1UL << 0));
    DMA1->HIFCR       |= (0x3FUL << 16);   // Stream6 flag temizle
    DMA1_Stream6->M0AR = (uint32_t)tx_packet;
    DMA1_Stream6->NDTR = PACKET_TOTAL_SIZE;
    DMA1_Stream6->CR  |= (1UL << 0);       // DMA enable
}



void DMA1_Stream6_IRQHandler(void) {
    if (DMA1->HISR & (1UL << 21)) {        	// Stream6 TC flag
        DMA1->HIFCR |= (1UL << 21);
        									// Gönderim bitti, uart_tx_ready sıfırla
        circ_index = 0;						// buffer index başa dönsün
        uart_tx_ready = 0;
    }
}

