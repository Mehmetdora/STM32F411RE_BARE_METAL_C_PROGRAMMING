/*
 * DoorLockApp.c
 *
 *  Created on: 9 Mar 2026
 *      Author: mehmet_dora
 */


#include <stm32f4xx.h>
#include "LEDDriver.h"
#include "ButtonDriver.h"


#include <stdint.h>

// uygulamada kullanılacak state'lerin tanımlanması
typedef enum {
	STATE_LOCKED,
	STATE_ENTERING,
	STATE_UNLOCKED,
	STATE_ALARM
}APP_STATE_t;


static APP_STATE_t state;


// State'ler için ölçümlerde kullanılan değişkenler
uint32_t last_blink_time_entering = 0;
uint32_t last_blink_time_alarm = 0;
uint8_t current_pressed = 0;
uint8_t last_pressed = 0;

uint8_t correct_press_count = 3;
uint8_t current_press_count = 0;
uint32_t time_to_enter = 5000;
uint32_t last_entering_try_time = 0;

uint8_t wrong_try_to_enter = 0;


uint32_t unlocked_wait_time = 0;
uint32_t alarm_wait_time = 0;


void DoorLockApp_init(void){		// Uygulamanın çalışması için gereken bağımlılıkların ayarlanması

	ButtonDriver_init();
	LEDDriver_init();
	state = STATE_LOCKED;

}



void DoorLockApp_start(void){		// Sürekli çalışacak app


	uint32_t now = HAL_GetTick();
	uint8_t button_pressed = 0;
	current_pressed = ButtonDriver_read();

	if(current_pressed == 0 && last_pressed == 1){	// Her butona basılmasını kontrol etme
		button_pressed = 1;
	}


	/*
	 * Her state kendi içinde sürekli çalışabilmeli, yani bit bağımlılığa yada tetiklemeye bağlı kalmadan da
	 * sürekli olarak çalışabilmeli.
	 *
	 * Başka bir state e geçebilmek için mevcut state durumunda iken bir event kontrolü yapılarak koşulun
	 * sağlanması halinde state güncellenmeli
	 *
	 * Her state için olası tüm durumlar düşünülmeli ve buna göre bu state geçişleri kodlanmalı
	 *
	 * State'ler arasında geçişler yapılırken önceki ve sonraki state için kontrol edilecek değişkenlerin
	 * değerleri doğru yerlerde set veya reset edilmeli
	 *
	 *
	 */
	switch(state){			// Her state için özel çalışacak kodların tanımlanması

	case STATE_LOCKED:

		LEDDriver_off();
		if(button_pressed){
			state = STATE_ENTERING;
			last_blink_time_entering = now;
			last_entering_try_time = now;
		}

		break;



	case STATE_ENTERING:


		if(button_pressed){
			current_press_count++;
		}


		if((now - last_entering_try_time) > time_to_enter){
			if(current_press_count == correct_press_count){
				state = STATE_UNLOCKED;
				current_press_count = 0;
				unlocked_wait_time = now;
			}else{
				wrong_try_to_enter++;
				current_press_count = 0;
				if(wrong_try_to_enter == 3){
					state = STATE_ALARM;
					last_blink_time_alarm = now;
					alarm_wait_time = now;
					wrong_try_to_enter = 0;

				}
			}
			last_entering_try_time = now;
		}




		if((now - last_blink_time_entering) > 500){
			LEDDriver_toggle();
			last_blink_time_entering = now;
		}
		break;



	case STATE_UNLOCKED:

		LEDDriver_on();
		if((now - unlocked_wait_time) > 10000){
			state = STATE_LOCKED;
		}
		break;



	case STATE_ALARM:

		if((now - last_blink_time_alarm) > 100){
			LEDDriver_toggle();
			last_blink_time_alarm = now;
		}

		if((now - alarm_wait_time) > 5000){
			state = STATE_LOCKED;
		}
		break;


	}



	last_pressed = current_pressed;


}
