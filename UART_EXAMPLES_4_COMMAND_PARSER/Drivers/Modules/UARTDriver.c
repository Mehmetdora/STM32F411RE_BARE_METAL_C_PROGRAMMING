/*
 * UARTDriver.c
 *
 *  Created on: 11 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include "RingBuffer.h"


#define RING_BUFFER_SIZE (64)

RingBuffer_t tx_rb = {0U};
static uint8_t  tx_DataBuffer[RING_BUFFER_SIZE] = {0U};


RingBuffer_t rx_rb = {0U};
static uint8_t rx_DataBuffer[RING_BUFFER_SIZE] = {0U};



void UARTDriver_init(void){



	// Ring buffer init()
	ring_buffer_setup(&tx_rb, &tx_DataBuffer , RING_BUFFER_SIZE);
	ring_buffer_setup(&rx_rb, &rx_DataBuffer , RING_BUFFER_SIZE);


	RCC->APB1ENR |= (1UL << 17); 		// usart2 clock enable, AF7 için


	RCC->AHB1ENR |= (1UL << 0);			// A portunun clock enable
	// USART2 çıkışları A portundan sağlanacağı için bu port çıkışlarının clockunu enable et



	// İstenen GPIOA pinlerinin modunu AF olarak ayarlanması , PA2 ve PA3
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




	// Transmitter enable ve receiver enable yapmak

	USART2->CR1 |= (1UL << 2) | (1UL << 3);


	// USART2 RX interrupt enable, veri almaya hemen başlanabilir
	USART2->CR1 |= (1UL << 5);		// Bu bit RXENIE ile RX için interruot a izin verir
	NVIC_EnableIRQ(USART2_IRQn);	// Bu NVIC in bu interrupt için erişilebilmesini sağlar.


	USART2->CR1 |= (1UL << 13);		// Usart2 enable

}





void UARTDriver_send_string(char* data)
{
    if (data == NULL) { return; }				// veri yoksa bir şey gönderme

    while (*data != '\0')						// verinin sonuna gelen kadar her karakteri gönder
   {
        UARTDriver_send_byte((uint8_t)(*data));
        data++;									// sonraki karaktere geç
    }
}




void UARTDriver_send_byte(uint8_t byte)
{


    ring_buffer_write(&tx_rb, byte);
    /*
     * TX veri gönderme buffer ına byte yazılır,
     *
     */


    USART2->CR1 |= (1UL << 7);
    /*
     * Normalde gönderim yeri boş olduğunda interrupt tetiklenirdi ama bu flag kapalı olduğundan gerçekleşmiyordu.
     * Şimdi bu flag açılarak interrupt tetiklenmesi sağlanıyor, böylece zincirleme şekilde veriler gönderilmeye
     * başlanıyor.
     */
}





// buffer boşsa 0 döner, veri varsa bunu döner
uint8_t UARTDriver_read(void)
{
    uint8_t byte = 0U;
    ring_buffer_read(&rx_rb, &byte);		// 1 byte veri okunur, sonrasında read index bir sonraki indexi gösterecek şekilde ayarlanır.
    return byte;
}


/* -----------------------------------------------------------------------
 * UARTDriver_data_available
 *
 * RX buffer'da okunmayı bekleyen veri var mı?
 * Retval: 1 → veri var, 0 → buffer boş
 * ----------------------------------------------------------------------- */
uint8_t UARTDriver_data_is_available(void)
{
    return (ring_buffer_is_empty(&rx_rb) == 0U) ? 1U : 0U;		// eğer veri varsa true dönecek, yani read ile write aynı yeri göstermiyor.
}








void USART2_IRQHandler(void){


	// RX INTERRUPT
	// interrupt türü RXNE ise ve RX için interrupt enable edilmiş ise
	if((USART2->SR & (1UL << 5)) && (USART2->CR1 & (1UL << 5)) != RESET){

		/*
		 * RX interrupt başta enable edildiği için veri geldiği an tetikleniyor.
		 * Veri gelmesi ile birlikte RX buffer'ına yazılıyor.
		 *
		 * Gelen veri artık RX buffer ına yazıldıktan sonra bu veri main içinde okunarak kullanılabilir.
		 */





		ring_buffer_write(&rx_rb, (uint8_t)USART2->DR);





		return;
	}




	// TX INTERRUPT
	if(USART2->SR & (1UL << 7) && (USART2->CR1 & (1UL << 7)) != RESET){


		/*
		 * TX buffer ına yazılan veri olup olmadığına bakılır, eğer veri varsa bu veri DR register ına yazılır,
		 * eğer veri yoksa tekrar arka arkaya sürekli boş yere interrupt tetiklenmesin diye flag tekrar kapatılır.
		 */

		uint8_t tx_data = 0U;
		if(ring_buffer_read(&tx_rb, &tx_data) == 1U){

			USART2->DR = (uint32_t)tx_data;		// DR ye yazılan veri gönderilip de DR boş kaldığı an tekrar interrupt tetiklenecek

		}else{

			USART2->CR1 &= ~(1UL << 7);
		}

		return;

	}




}



