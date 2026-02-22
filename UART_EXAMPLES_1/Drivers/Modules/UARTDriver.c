/*
 * UARTDriver.c
 *
 *  Created on: 22 Şub 2026
 *      Author: mehmet_dora
 */

#include <UARTDriver.h>
#include "stm32f4xx.h"

void uart_init(void){


	RCC->APB1ENR |= (1UL << 17); 		// usart2 clock enable, AF7 için


	RCC->AHB1ENR |= (1UL << 0);			// A portunun clock enable
	// USART2 çıkışları A portundan sağlanacağı için bu port çıkışlarının clockunu enable et



	// İstenen GPIOA pinlerinin modunu AF olarak ayarlanması , PA2 ve PA3
	GPIOA->MODER &= ~(3UL << (2*2));
	GPIOA->MODER &= ~(3UL << (2*3));

	GPIOA->MODER |= (2UL << (2*2));
	GPIOA->MODER |= (2UL << (2*3));



	// haberleşme için ilgili pinlerin high speed olarak ayarlanması , PA2 ve PA3
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
	 */

	USART2->BRR = (22UL << 4) | (13UL);		// eski değerlerlerin kalmaması için direkt = kullan




	// Transmitter enable ve receiver enable yapmak

	USART2->CR1 |= (1UL << 2) | (1UL << 3);


	USART2->CR1 |= (1UL << 13);		// Usart2 enable




}



void uart_send_char(uint8_t data){

	USART2->DR = data;
	while( !(USART2->SR & (1UL << 6)) ){};		// Önceki veri gönderilip yeniden gönderilmeye hazıl olunana kadar bekle
}

void uart_send_string(char* string){
	while(*string){
		uart_send_char(*string++);
	}
}

uint8_t uart_read_char(){

	uint8_t temp;

	while(! (USART2->SR & (1UL << 5))){};		// Receive data register okumak için hazır olana kadar bekle

	temp = USART2->DR;
	return temp;



}

