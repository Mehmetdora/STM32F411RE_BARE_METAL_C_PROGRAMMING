/*
 * I2C_Driver.h
 *
 *  Created on: Apr 3, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_I2C_DRIVER_H_
#define MODULES_I2C_DRIVER_H_

void I2C1_init(void);
void I2C1_BurstRead(char saddr, char maddr, int n, char* data);
void I2C1_BurstWrite(char saddr, char maddr, int n, char* data);


#endif /* MODULES_I2C_DRIVER_H_ */
