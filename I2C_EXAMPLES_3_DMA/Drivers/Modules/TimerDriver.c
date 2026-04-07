/*
 * TimerDriver.c
 *
 *  Created on: Apr 5, 2026
 *      Author: mehmet_dora
 */


#include "stm32f4xx.h"
#include "stdint.h"
#include "TimerDriver.h"
#include "I2C_Driver.h"
#include "I2C_DMA_Driver.h"




#define MPU6050_ADDR  (0x68)
#define ACCEL_START   0x3B
#define GYRO_START    0x43

#define RAW_BUFFER_SIZE 14



volatile uint32_t counter = 0;


void TimerDriver_init(void){

	//__HAL_RCC_TIM2_CLK_ENABLE(); 	// Timer2 clock enable
	RCC->APB1ENR |= (1UL << 0);		// Timer2 clock enable


	// PRESCALER
	TIM2->PSC = (8400 - 1);	// APB1 clock değeri 84 MHz ve 1 ms için -> 8.400 x 10.000
	// 84.000.000 / 8.400 = 10000 hz = 0.1 ms
	// DİKKAT
	// PSC 16 bit tutabilir , yani max PSC değeri 65536 verilebilir !!!


	// Auto-Reloud
	// Kaça kadar saydıktan sonra başa döneceği
	TIM2->ARR = (10000 - 1);		// Her 10000 saymada bir başa dönsün


	// OPSİYONEL AYAR
	// TAM burada PSC ve ARR değerleri hemen donanıma yüklenmez , donanım bir update event bekler
	// yani bir cycle boşa gitmiş olur , bunu engellemek için ilk başlamada bu değerlerinin yüklenmesini
	// sağlayan TIM2->EGR(Event Generation) ın UG flag i set edilmelidir
	// Bu şekilde elle event oluşturma ile ilk overflow sırasındaki sayım tam olarak 0'dan başlaması
	// garanti edilmiş olunur.
	TIM2->EGR |= (1UL << 0);
	TIM2->SR &= ~(1UL << 0);	// Olmaması gereken ilk interrupt'ın tetiklenmesini engelleme


	// Interrupt Enable
	TIM2->DIER |= (1UL << 0);	// 0. bit interrupt kontrol eder



	NVIC_SetPriority(TIM2_IRQn, 2);
	NVIC_EnableIRQ(TIM2_IRQn);	// NVIC ile TIM2 nin interrupt özelliğini aç, CPU'ya erişebilsin
	// Buraya TIM2 enum adı yazılır ki bu sayede TIM2 nin asıl numarası CPU'ya bildirilebilsin


	TimerDriver_enable();

}


void TimerDriver_enable(void){
	TIM2->CR1 |= (1UL << 0);	// 0. bit olan CEN(Counter Enable) bitini set ederek saymaya başlamasını sağla
}

void TimerDriver_disable(void){
	TIM2->CR1 &= ~(1UL << 0);	// 0. bit olan CEN(Counter Enable) bitini clear ederek TIM2 yi durdur
}

void TimerDriver_set_period(uint32_t period){

	TimerDriver_disable();
	TIM2->ARR = period - 1;
	TimerDriver_enable();
}



void TIM2_IRQHandler(void){




	if(TIM2->SR & (1UL << 0)){	// TIM2->SR ın 0. biti UIF flag i dir. Bu set olmuşsa interrupt olmuş demek olur

		// Bu flag counter overflow olduktan ve sıfırlandıktan sonra set edilir.

		// Bu SR içindeki UIF flag'i elle tekrar clear edilmelidir, bu sayede sonraki flag yakalanabilsin.
		// Bu flag yazılım tarafından güncellenmesi gerekir.
		TIM2->SR &= ~TIM_SR_UIF;




		// Her timer interrupt tetiklendiğinde yani 1 sn aralıklarla sensör okuma başlatılsın
		I2C1_BurstRead(MPU6050_ADDR, ACCEL_START, RAW_BUFFER_SIZE);



		// Her 30 saniyede bir buffer da toplanan veriler esp32 ye aktarılacak
		counter++;

		GPIOA->ODR ^= (1UL << 5);

	}


}




