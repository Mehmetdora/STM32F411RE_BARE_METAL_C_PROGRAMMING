/*
 * SysTickDriver.c
 *
 *  Created on: 16 Şub 2026
 *      Author: mehmet_dora
 */



#include "stm32f4xx_hal.h"

volatile uint32_t systick_counter = 0;
// main ve interrupt dosyasında kullanılacak global sayaç değişkenidir.
// volatile kullanımı derleyiciye bu değişkenin donanım tarafından değiştirileceğini belirtir
// Bu değişken her interrupt oluştuğunda geçen zamanı(ms cinsinden) tutan sayaçtır.
// Bu değişken stm32f4xx_it.c içindeki interrupt ISR içinde arttırılıyor


/*
 * SYSTICK TİMER HAKKINDA
 *
 * SysTick timer stm32 tarafından geliştirilen bir timer değildir, stm32 nin kullandığı çipin çekirdeği
 * üzerinde gelen ve bu çekirdeğin geliştiricisi olan ARM tarafından hazır olarak gelen bir timer'dır.
 *
 * Aslında Systick timer hakkında detaylı bilgiler ARM ın belgelerine bakılarak daha detaylı öğrenilebilir.
 *
 * Systick timer üzerinde bir adet 24 bitlik geriye doğru sayan bir sayıcı bulunur.
 */



void SysTickDriver_init(void){


	SysTick->CTRL = 0;				// Önce kapat


	// SysTick->LOAD İle YApılanlar;
	// - CPU'nun çalışma frekansı SystemCoreClock ile alınır, bu değer CPU'nun 1sn için çalıştırması gereken clock cycle sayısıdır
	// - Systick Reload register max 24 bit değer tutabildiği için 16.777.215'ten fazla frekanslar ile interrupt kullanarak
	//   sayma yapılamaz, bunu çözmek için yazılımsal olarak ek counter ile sayılmalıdır. Mesela 16MHz çalışan bir CPU max
	//   16.000.000 clock cycle sayar 1 sn için. Biz ise bu 16MHz değeri 24 bitlik yere sığabildiği için 1sn 'lik aralıklar ile
	//   interrupt üretebiliriz.
	// - Eğer 1 ms lik aralıklar ile interrupt üreterek daha hassas bir zaman sayımı yapacaksak LOAD register'ına yüklediğimiz
	//   değeri 1000'e bölmeliyiz ki CPU'nun 1 sn için kullandığı clock cycle'ın 1000'de biri olan 1 ms'lik interrupt'lar tetikleyebilelim
	SysTick->LOAD = (SystemCoreClock/1000) - 1 ;	// 1ms aralıklarla interrupt üretilir.

	// LOAD REGİSTER
	/*
	 * Sayıcının hangi değerden geriye doğru sayacağının belirlendiği bitlerdir. Sayıcı bu değerden
	 * geriye sayacaktır ve her 0 a ulaştığında tekrar bu değerden geriye saymaya devam edecektir
	 * Sayma 0 dan başladığı için bu değeri 1 eksik atamalıyız.
	 */

	SysTick->VAL = 0;
	// CURRENT VALUE REGİSTER
	/*
	 * Bu register sayma sırasında anlık olarak sayıcının değerinin okunabileceği register'dır.
	 * Sayma işleminin en baştan başlaması için bu değeri 0 olarak başlatıyoruz.
	 */


	SysTick->CTRL |= (1UL << 0);	// Systick timer enable edilmesi
	SysTick->CTRL |= (1UL << 1);	// systick timer sayacı 0'a ulaştığı zaman oluşan interrupt'ın aktif edilmesi
	SysTick->CTRL |= (1UL << 2);	// Bu bit 1 ise systick timer frekansı doğrudan AHB hattının frekansına eşitlenmiş olur
									// bu bit 0 yapıldığında systick timer frekansı AHB/8 frekansına eşit olmuş olur


	/*
	 * Bu fonksiyon main içinde çağrıldıktan sonra SysTick çalışmaya başlar,
	 * SysTick interrupt, her SysTick->VAL değeri 0 a geldiğinde COUTFlAG'ın 1 olması ile tetiklenir.
	 * COUNTFLAG değeri donanım tarafından otomatik 0 yapılır. SysTick tekrar SysTick->LOAD değerinden
	 * 0 a doğru saymaya başlar.
	 *
	 *
	 */

}


void Systick_Delay(uint32_t delay){

	systick_counter = 0;
	while(systick_counter < delay){
		// bu fonksiyon çağrıldığında counter değeri istenen delay değerine eşit olana
		// kadar burada kalarak tam bir HAL_Delay fonksiyonu olarak çalışması sağlanır
	}

}










