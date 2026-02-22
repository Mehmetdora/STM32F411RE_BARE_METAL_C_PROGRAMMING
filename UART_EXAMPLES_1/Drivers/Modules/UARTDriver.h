/*
 * UARTDriver.h
 *
 *  Created on: 22 Şub 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_UARTDRIVER_H_
#define MODULES_UARTDRIVER_H_

#include <stdint.h>


void uart_send_char(uint8_t data);
void uart_send_string(char* string);
uint8_t uart_read_char();

void uart_init(void);





#endif /* MODULES_UARTDRIVER_H_ */
