/*
 * UARTDriver.h
 *
 *  Created on: 11 Mar 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_UARTDRIVER_H_
#define MODULES_UARTDRIVER_H_

#include <stdint.h>


void    UARTDriver_init(void);

void    UARTDriver_send_byte(uint8_t byte);
void    UARTDriver_send_string(char* data);

uint8_t UARTDriver_read(void);
uint8_t UARTDriver_data_is_available(void);

#endif /* MODULES_UARTDRIVER_H_ */
