/*
 * I2C_Driver.h
 *
 *  Created on: Apr 6, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_I2C_DRIVER_H_
#define MODULES_I2C_DRIVER_H_


#include "stdint.h"

void I2C1_init(void);
void I2C1_BurstWrite(uint8_t saddr, uint8_t maddr, uint8_t n, uint8_t* data);
void I2C1_BurstRead(uint8_t saddr, uint8_t maddr, uint8_t n);
void MPU6050_wakeup(uint8_t mpu_saddr);

#endif /* MODULES_I2C_DRIVER_H_ */
