#include "stm32f4xx.h"
#include <stdint.h>

#include <LedDriver.h>
#include <SysTickDriver.h>


int main(void)
{
	SysTickDriver_init();
    LedDriver_init();

    while (1)
    {


    	// SysTickDriver.h dosyasında tanımlanan systick_counter değerini kontrol et, 1 ms az ise işlem yap
    	// Bu systick_counter değişkeni interrupt fonksiyonu içerisinde arttırılıyor. Interrupt fonksiyonu
    	// her 1 ms'de bir tetikleniyor.
        if (systick_counter >= 1000)
        {
            systick_counter = 0;
            LedDriver_toggle();  // LED toggle
        }





    }
}
