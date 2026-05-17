/*
 * ADCDriver.c
 *
 *  Created on: 28 Şub 2026
 *      Author: mehmet_dora
 */

#include <stm32f4xx.h>
#include "ADCDriver.h"
#include "IIR_DSP.h"


#define IIR_ALPHA 0.05f
FirstOrderIIR filt;





void ADC_init(int resolution){


	// RCC init()
	RCC->APB2ENR |= (1UL << 8);	// ADC1 için clock enable
	RCC->AHB1ENR |= (1UL << 0);	// GPIOA için clock enable


	GPIOA->MODER |= (3UL << 0);	// PA0 analog mode set



	// Chanel Select
	ADC1->SQR3 = 0;		//PA0 kullanılacağı için 0. chanel ayarlandı, datasheet de listesi var

	// Resolution
	uint32_t resolution_bit = 0; 	// input olarak belirlenen değere göre resolution seçimi
	switch(resolution){
	case 12:
		resolution_bit = 0;
		break;
	case 10:
		resolution_bit = 1;
		break;
	case 8:
		resolution_bit = 2;
		break;
	case 6:
		resolution_bit = 3;
		break;
	}
	ADC1->CR1 |= (resolution_bit << 24);

	// Sample time (Channel 0)
	ADC1->SMPR2 |= (7UL << 0);		// en yüksek sample modu seçildiği için daha doğru sonuçlar gelecektir fakat
									// daha uzun sürecektir.

	// Sequence Length
	ADC1->SQR1 &= ~(15UL << 20);	// Sıralı kanal ölçümleri için kaç tane kanal kullanılacağı

	// Kaç tane kullanılacaksa her birinin hangi sıra ile yapılması gerektiğinin belirlenmesi
	ADC1->SQR3 &= ~(31UL << 0);		// İlk sequence için chanel 0 için hepsini sıfırla
	// SQR register'ına hangi chanel kullanılacaksa onun numarası yazılır


	// EOC interrupt enable
	ADC1->CR1 |= (1UL << 5);	// interrupt sonunda tekrar clear edilmesi gerekir.

	// Enable NVIC to interrupt
	NVIC_EnableIRQ(ADC_IRQn);



	// Enable ADC1 first time , and wait a bit
	ADC1->CR2 |= (1UL << 0) | (1UL << 1);
	HAL_Delay(1);


	ADC1->CR2 |= (1UL << 1);

	// Start ADC, SWSTART
	ADC1->CR2 |= (1UL << 30);





	// IIR Filter init

	FirstOrderIIR_Init(&filt, IIR_ALPHA);

}






// ADC Interrupt ISR

void ADC_IRQHandler(void){


	// interrutp EOC için tetiklenmiş ise değeri belleğe aktar
	if(ADC1->SR & (1UL << 1)){

		analog_val = ADC1->DR;
		// EOC tekrar manual clear etmeye gerek yoktur çünkü RM de yazdığı gibi DR ile değer okunduğunda
		// EOC da clear ediliyor.



		FirstOrderIIR_Update(&filt, analog_val);
	}


}
