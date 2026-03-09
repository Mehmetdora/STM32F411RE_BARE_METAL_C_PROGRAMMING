/*
 * LED_APP.c
 *
 *  Created on: 9 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include "LEDDriver.h"
#include "ButtonDriver.h"



typedef enum {
	LED_STATE_ON,
	LED_STATE_OFF,
	LED_STATE_BLINK
}LED_STATE_t;



static LED_STATE_t state;
uint8_t last_pressed = 0;
uint8_t current_pressed = 0;
uint32_t last_blink_tick = 0;


void LED_App_Init(void){

	LEDDriver_init();
	ButtonDriver_init();
	state = LED_STATE_OFF;

}



void LED_App_Run(void){



	current_pressed = ButtonDriver_read();

	// Butona basılmış ve sonrasında tam çekilirken falling edge yakalama
	if(current_pressed == 0 && last_pressed == 1){

		switch(state){

		case LED_STATE_OFF:
			LEDDriver_on();
			state = LED_STATE_ON;

			break;

		case LED_STATE_ON:
			LEDDriver_off();
			last_blink_tick = HAL_GetTick();	// Blink durumuna geçmeden önce last_blink_tick şu anki zamanı alır ki 300 ms geçtiği ölçülebilsin
			state = LED_STATE_BLINK;

			break;


		case LED_STATE_BLINK:
			LEDDriver_off();
			state = LED_STATE_OFF;

			break;
		}


	}


	if(state == LED_STATE_BLINK){
		uint32_t now = HAL_GetTick();
		if(now - last_blink_tick > 300){		//Eğer blink state'ine geçilmişse, 300 ms de bir led toggle ediliyor.
			LEDDriver_toggle();
			last_blink_tick = now;
		}
	}

	last_pressed = current_pressed;		// current_pressed butona basılmama durumunu tuttuğundan, yeni durum için güncelle


}
