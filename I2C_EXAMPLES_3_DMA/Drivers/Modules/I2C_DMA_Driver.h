/*
 * I2C_DMA_Driver.h
 *
 *  Created on: Apr 4, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_I2C_DMA_DRIVER_H_
#define MODULES_I2C_DMA_DRIVER_H_

#include "stdint.h"

#define RAW_BUFFER_SIZE 14
#define CIRC_SIZE        30

extern volatile uint8_t CIRC_BUFFER[CIRC_SIZE * 12];
extern volatile uint8_t RAW_BUFFER[RAW_BUFFER_SIZE];
extern volatile uint8_t uart_tx_ready;
extern volatile uint16_t circ_index;

void I2C1_rx_DMA_stream0_init(void);
void DMA_i2c_rx_start(void);

#endif /* MODULES_I2C_DMA_DRIVER_H_ */
