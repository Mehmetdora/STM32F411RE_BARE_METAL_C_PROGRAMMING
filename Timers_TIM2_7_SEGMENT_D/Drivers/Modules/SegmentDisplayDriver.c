/*
 * SegmentDisplayDriver.c
 *
 *  Created on: 18 Şub 2026
 *      Author: mehmet_dora
 */


#include <stdint.h>
#include "stm32f4xx_hal.h"



#define LOW 0
#define HIGH 1

// 7 segment display bacaklarının anlamlı rakamlar gösterebilmesi için yanma kombinasyonları
const uint32_t counts[10] = {
		0x3F,	// 0
		0x06,	// 1
		0x5B,	// 2
		0x4F,
		0x66,
		0x6D,
		0x7D,
		0x07,
		0x7F,	// 8
		0x6F,	// 9
	};

// 7 segment displayin hangi bacağının kaçıncı A Portuna bağlı olduğu bilgisi
uint8_t segment_pins[7] = {		// Port A numaraları
    6,   // a -> PA6
    12,  // b -> PA12
    9,   // c -> PA9
    10,  // d -> PA10
    8,   // e -> PA8
    7,   // f -> PA7
    11   // g -> PA11
};


void digitalWrite(uint8_t pin, uint8_t val);


// 7 segment display için gerekli port initialization fonk.
void SegmentDisplayDriver_init(void){


	RCC->AHB1ENR |= (1UL << 0);	// GPIOA clock enable


	// İlgili pinlerin clear edilmesi
	GPIOA->MODER &= ~(3UL << (2*6));
	GPIOA->MODER &= ~(3UL << (2*7));
	GPIOA->MODER &= ~(3UL << (2*8));
	GPIOA->MODER &= ~(3UL << (2*9));
	GPIOA->MODER &= ~(3UL << (2*10));
	GPIOA->MODER &= ~(3UL << (2*11));
	GPIOA->MODER &= ~(3UL << (2*12));


	// ilgili pinlerin output yapılması
	GPIOA->MODER |= (1UL << (2*6));
	GPIOA->MODER |= (1UL << (2*7));
	GPIOA->MODER |= (1UL << (2*8));
	GPIOA->MODER |= (1UL << (2*9));
	GPIOA->MODER |= (1UL << (2*10));
	GPIOA->MODER |= (1UL << (2*11));
	GPIOA->MODER |= (1UL << (2*12));

}



// tüm 7 segment display bacaklarını açıp kapatan fonksiyon
void SegmentDisplayDriver_toggle(void){

	GPIOA->ODR ^= (1UL << 6);
	GPIOA->ODR ^= (1UL << 7);
	GPIOA->ODR ^= (1UL << 8);
	GPIOA->ODR ^= (1UL << 9);
	GPIOA->ODR ^= (1UL << 10);
	GPIOA->ODR ^= (1UL << 11);
	GPIOA->ODR ^= (1UL << 12);


}



void SegmentDisplayDriver_disable(void){
	for (int i = 0; i < 7; i++)
	{
		digitalWrite(segment_pins[i], LOW);
	}

}



// Aldığı sayıya göre 7 segment display görünümünü ayarlayan fonk.
void SegmentDisplayDriver_display(uint32_t value){

	// A10 -> d
	// A8 -> e
	// A9 -> c
	// A7 -> f
	// A6 -> a
	// A12 -> b
	// A11 -> g

	// g - f - e - d - c - b - a
	// 11  7   8   10  9   12  6

	uint8_t mask = counts[value];

	for (int i = 0; i < 7; i++)
	{
		if (mask & (1 << i))
			digitalWrite(segment_pins[i], HIGH);
		else
			digitalWrite(segment_pins[i], LOW);
	}



}




// Her bir port pinini ayarlaryan fonksiyon
void digitalWrite(uint8_t pin, uint8_t val)
{

	if(val){
		GPIOA->ODR |= (1UL << pin);
	}else{
		GPIOA->ODR &= ~(1UL << pin);
	}

}


