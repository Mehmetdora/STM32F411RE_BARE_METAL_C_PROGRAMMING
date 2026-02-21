/*
 * PWMDriver.c
 *
 *  Created on: 20 Şub 2026
 *      Author: mehmet_dora
 */


#include "stm32f4xx_hal.h"

void PWMDriver_on(void);

void PWMDriver_init(void){


	// ENABLE GPIOA CLOCK
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	// RCC->AHB1ENR |= (1UL << 0);		// Üstteki ile aynı şey


	// ENABLE TIMER2 CLOCK
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	// RCC->APB1ENR |= (1UL << 0);		// Üstteki ile aynı şey




	// PORTu ALTERNATİF FONK OLARAK AYARLA
	GPIOA->MODER &= ~(3UL << (2*5));   // temizle
	GPIOA->MODER |= (2UL << (2*5));
	// TIM2 destekleyen bir pin seçilmeli
	// PA0 datasheet içindeki alternate fonk. tablosunda desteklediği için kullanılabilir


	// SEÇİLEN PİN İÇİN AF BİLGİSİNİ TANIMLAMA
	GPIOA->AFR[0] &= ~(0xF << (4*5));  // AF bits temizle
	GPIOA->AFR[0] |= (1UL << (4*5));
	// Buradaki AFR[0] seçilmesinin nedeni PA0 pininin numarası 8'den küçük olduğu için AFR[0] seçildi
	// eğer 8den büyük olsaydı AFR[1] seçilmesi gerekirdi.
	// AFR register içinde her pin için AF seçimi 4 bit ile yapıldığı için 4*0 yapılarak aslında ilk 4'lük seçildi.



	// CLOCK SİNYALİ AYARLAMA
	TIM2->PSC = 84-1;

	TIM2->ARR = 20000-1;		// SERVO KONTROLLERİ İÇİN BU DEĞERİN AYARLANMASI GEREKİR HZ DEĞERİ




	// TIM2 HANGİ MOD İLE ÇALIŞACAĞI

	// CH1/CH2 ---> CCMR1 ile ayarlanır, CCMR1 içinde CH1 ve CH2 içiden ayarlama bitleri 1 ve 2 ile buna göre ayrılır
	// CH3/CH4 ---> CCMR2 ile ayarlanır, aynı şekilde CH2 ve CH3 için de 3 ve 4 olarak ayrılır.
	TIM2->CCMR1 &= ~(7 << 4);	// öncesinde ilgili yerin clear edilmesi.
	TIM2->CCMR1 |= (6UL << 4);	// buradaki 6UL -> 110 demektir , bu ise PWM 1 modunu seçmek anlamına geliyor.
	// PWM modu şuna göre karşılaştırır
	/*
		CNT < CCR1 → output HIGH
		CNT ≥ CCR1 → output LOW
	 */



	// TIM2 PWM PRELOAD ENABLE
	TIM2->CCMR1 |= (1UL << 3);	// 3. bitteki preload ayarını enbable yaparak sıfırdan başlamasını sağla



	// TIM2 İÇİN CAPTURE/COMPARE MODUNU ENABLE YAP
	TIM2->CCER |= (1UL << 0);	// 0. bit C/C için enable ayarı içindir, set edildi.






	// TIM2 ARR İÇİN PRELOAD ENABLE
	TIM2->CR1 |= (1UL << 7);


	// UPDATE EVENT GENERATION eNABLE YAP
	// önceki counter değerlerinin sıfırlanması ile tüm counter'ların 0'dan saymaya başlamasını sağlar.
	// yani en baştan tüm timer'ı başlat
	TIM2->EGR |= (1UL << 0);	// hemen event üret ve PWM counter'ları sıfırlanarak başlasın




	// START TİMER COUNTİNG
	PWMDriver_on();	// 0. bit CEN dir ve tüm timer'ın saymaya başlamasını enable yapar. Sistemi başlatır.



}


void PWMDriver_on(void){
	TIM2->CR1 |= (1UL << 0);
}


void PWMDriver_off(void){
	TIM2->CR1 &= ~(1UL << 0);
}
void PWMDriver_set_periot(uint32_t val);


