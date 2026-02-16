/*
 * SysTickDriver.c
 *
 *  Created on: 16 Şub 2026
 *      Author: mehmet_dora
 */



#include "stm32f4xx_hal.h"

volatile uint32_t systick_counter = 0;		// main ve interrupt dosyasında kullanılacak global sayaç değişkeni

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

	SysTick->VAL = 0;


	SysTick->CTRL |= (1UL << 0);	// Systick enable et
	SysTick->CTRL |= (1UL << 1);	// systick interrupt ı enable et
	SysTick->CTRL |= (1UL << 2);	// processor(internal) clock kullan


	/*
	 * Bu fonksiyon main içinde çağrıldıktan sonra SysTick çalışmaya başlar,
	 * SysTick interrupt, her SysTick->VAL değeri 0 a geldiğinde COUTFlAG'ın 1 olması ile tetiklenir.
	 * COUNTFLAG değeri donanım tarafından otomatik 0 yapılır. SysTick tekrar SysTick->LOAD değerinden
	 * 0 a doğru saymaya başlar.
	 *
	 *
	 */









}

