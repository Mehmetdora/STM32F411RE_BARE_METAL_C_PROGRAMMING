/*
 * RingBuffer.c
 *
 *  Created on: 11 Mar 2026
 *      Author: mehmet_dora
 */

#include <stm32f4xx.h>
#include "RingBuffer.h"
#include <stdint.h>

void ring_buffer_setup(RingBuffer_t* rb, uint8_t* buffer, uint16_t size){

	rb->buffer = buffer;
	rb->read_index = 0;
	rb->write_index = 0;
	rb->size = size ;
}



uint8_t ring_buffer_is_empty(RingBuffer_t* rb){
	return (rb->read_index == rb->write_index) ? 1 : 0;
}



uint8_t ring_buffer_read(RingBuffer_t* rb , uint8_t* byte){




	if(rb->read_index == rb->write_index){	// buffer boşsa bitir
		return 0;
	}

	uint16_t next_r_index = ring_buffer_next_index(rb->read_index, rb->size);

	*byte = rb->buffer[rb->read_index];
	rb->read_index = next_r_index;			// güncel index'teki veriyi gönderdikten(okuduktan) sonra read index i arttır

	return 1;
}



uint8_t ring_buffer_write(RingBuffer_t* rb, uint8_t byte){


	uint16_t next_w_index = ring_buffer_next_index(rb->write_index, rb->size);	// sonraki write indexi al

	if(next_w_index == rb->read_index){			// Eğer read index ve sonraki write index eşit olacaksa bu buffer dolu demek

		//rb->read_index = ring_buffer_next_index(rb->read_index, rb->size);
		/*
		 * Bu yukarısaki satır ile buffer içindeki en eski verilerin üzerine yazılarak yeni gelen verilerin her zaman
		 * buffer a yazılmasını sağlar.
		 */



		return 0;
		/*
		 * Return 0  ile gelen yeni veri yazılmayacak , bu sayede en eski verilerin üzerine yazılması engellenmiş olunuyor.
		 */

	}

	rb->buffer[rb->write_index] = byte;		// Veriyi yaz ve indexi arttır sonraki gelen verinin oraya yazılması için
	rb->write_index = next_w_index;
	return 1;

}



// Eğer ring buffer sona gelmişse tekrar index değerlerinin baştan başlaması için yeni index i verir.
uint16_t ring_buffer_next_index(uint16_t current, uint16_t size) {
    if (current + 1 >= size) {
        return 0;        // sona gelindi, başa dön
    } else {
        return current + 1;
    }
}
