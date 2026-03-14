/*
 * RingBuffer.h
 *
 *  Created on: 11 Mar 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_RINGBUFFER_H_
#define MODULES_RINGBUFFER_H_

typedef struct RingBuffer_t{
	uint8_t* buffer;
	uint16_t size;
	uint16_t read_index;
	uint16_t write_index;
}RingBuffer_t;



void ring_buffer_setup(RingBuffer_t* rb, uint8_t* buffer, uint16_t size);
uint8_t ring_buffer_is_empty(RingBuffer_t* rb);
uint8_t ring_buffer_write(RingBuffer_t* rb, uint8_t byte);
uint8_t ring_buffer_read(RingBuffer_t* rb , uint8_t* byte);
uint16_t ring_buffer_next_index(uint16_t current, uint16_t size);






#endif /* MODULES_RINGBUFFER_H_ */
