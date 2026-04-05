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

typedef struct { int16_t ax, ay, az, gx, gy, gz; } Sample_t;
extern volatile Sample_t CIRC_BUFFER[CIRC_SIZE];
extern volatile uint8_t ROW_BUFFER[RAW_BUFFER_SIZE];
extern volatile uint8_t uart_tx_ready;
extern volatile uint8_t circ_index;

void I2C_DMA_init(void);
void I2C1_rx_DMA_stream0_init(void);
void I2C1_BurstRead_Blocking(uint8_t saddr, uint8_t maddr, uint8_t n);
void DMA_i2c_rx_start(void);

#endif /* MODULES_I2C_DMA_DRIVER_H_ */
