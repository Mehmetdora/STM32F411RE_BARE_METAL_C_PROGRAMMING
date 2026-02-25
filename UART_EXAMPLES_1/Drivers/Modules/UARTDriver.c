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

	/*
	 * POLLING BASE UART -> while içinde sürekli beklemek polling yöntemidir. İşlemciyi gereksiz yorar.
	 *	Polling yöntemi ile işlemci sürekli 6. bitin kontrolünü yapar, sistem kilitlenir.
	 *	Bu yöntem bu nedenle yoğun işler ve gerçek zamanlı sistemler için kullanılamaz.
	 *
	 *
	 *
	 *
	 *
	 * Bu kodda biz init() kodunda belirlediğimiz gibi 8 bitlik datayı DR register'ına yazıyoruz.
	 * DR register'ı arka planda veriyi paketler ve gönderir. Yani bitleri iletişim hattına süren UART'tır.
	 * Yani veriyi -> Start_bit- data(8bit) - stop_bit şeklinde paketler
	 *
	 * Sonrasındaki while içinde SR register'ının 6. biti Transmission Complete 'dir.
	 * Anlamı ise DR register tamamen boş, yeni veri koyabilirsin demektir.
	 * Sonuç olarak biz bu while döngüsü ile DR register'ına koyduğumuz veri gönderilip DR register
	 * boşalana kadar beklemesini sağlıyoruz.
	 *
	 * UART donanımı max 8-9 bit gönderebildiği için bu fonksiyon göndermek istediğimiz veriyi
	 * 8-9 bitlik parçalar halinde göndermemizi sağlar. Başka bir fonksiyon içinde bu fonk.
	 * kullanılarak tüm veri parça parça gönderilebilir.
	 *
	 */
}







void uart_send_string(char* string){
	while(*string){
		uart_send_char(*string++);
	}

	/*
	 * Bu fonksiyon ise göndermek istediğimiz tam verinin(string ifade) önceki parça parça gönderen
	 * fonksiyon ile gönderilmesini sağlar. Yani bu fonksiyon ile direkt istediğimiz veriyi göndermiş oluruz
	 *
	 * Göndereceğimiz veri string olduğundan string ifadeler de bir array gibidir. Kendi isimleri aslında
	 * adreslerini gösterir. String ifadelerin sonunda \0 karakteri bulunur ve bunu kullanarak string in
	 * sonuna geldiğimizi biliriz.
	 *
	 * While döngüsü içinde de char pointer türünde aldığımız string ifadeyi tek tek her bir karakterini
	 * pointer ile dönerek her karakteri yukarıdaki fonksiyon ile iletiyoruz. Böylece tüm string iletilmiş
	 * oluyor. String sonundaki \0 a gelindiğinde de döngüden çıkılarak işlem bitiyor.
	 */
}






uint8_t uart_read_char(){

	uint8_t temp;

	while(! (USART2->SR & (1UL << 5))){};		// Receive data register okumak için hazır olana kadar bekle

	temp = USART2->DR;
	return temp;

	/*
	 * POLLING BASE UART -> while içinde sürekli beklemek polling yöntemidir. İşlemciyi gereksiz yorar.
	 *
	 * Bu fonksiyon ile DR register'ından gelen veriler okunur. while koşulundaki SR register'ının
	 * 5. biti RXNE(RX not empty) dir. Bu flag DR içinde okunmamış veri olduğunu belirtir. Buradaki
	 * while döngüsü ise biz veri okumaya başladığımızda RXNE flag set edilmemişse , yani okunmamış veri
	 * DR içinde yoksa beklemesini sağlar. Eğer veri gelirse yani flag set edildiğinde bu while döngüsünden
	 * çıkılır.
	 *
	 * Sonrasında DR register içindeki veri okunur. Bu okuma işlemi ile RXNE flag i otomatik clear edilir.
	 * Gelen veri artık CPU'dadır. Bundan sonra gelen veri kullanılabilir.
	 */



}

