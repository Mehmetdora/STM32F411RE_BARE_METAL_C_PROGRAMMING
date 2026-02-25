#include <stm32f4xx.h>
#include "UARTDriver.h"

int main(void){


	uart_init();

	uart_send_string_IT("HELLO\r\n");


	while (1)
	{


		if (rx_flag)	// RX tarafından veri gelmiş mi ? Sürekli kontrol et
		{
			uint8_t c = uart_read_char_IT();

			// Gelen karakteri olduğu gibi geri gönder.
			char buf[2] = {c, '\0'};
			uart_send_string_IT(buf);
		}


		/*
		 * Normalde TX ile oldğu gibi RX için de interrupt tetiklenmesi ile gelen veriler interrupt
		 * fonksiyonu içinde işlenerek hiç main içindeki döngüde sürekli kontrol edilmesi engellenebilir
		 * fakat bu yöntem sadece interrupt içinde gelen veri üzerinde basit işlemler yapılacaksa uygundur.
		 *
		 * Yoksa eğer gelen veri üzerinden az da olsa zaman alacak işler yapılacaksa bu main içinde yapıl-
		 * malıdır. Bu şekilde interrupt kodları fazla olmaz , ana döngü işini yapmaya hemen devam eder.
		 *
		 */

		// burada CPU SERBEST

	}


}
