/*
 * UART_DMA_Driver.h
 *
 *  Created on: Apr 7, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_UART_DMA_DRIVER_H_
#define MODULES_UART_DMA_DRIVER_H_

void UART_init(void);
void UART_tx_DMA_stream0_init(void);
void UART2_send_packet(volatile uint8_t* CIRC_BUFFER);

#endif /* MODULES_UART_DMA_DRIVER_H_ */
