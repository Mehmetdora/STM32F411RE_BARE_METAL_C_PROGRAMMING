/*
 * CommandParser.h
 *
 *  Created on: 16 Mar 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_COMMANDPARSER_H_
#define MODULES_COMMANDPARSER_H_

#include <stdint.h>
#include "RingBuffer.h"
void CommandParser_RX_CMD_Complete_callback(RingBuffer_t* rx_rb);
uint8_t parameter_control(char* param1, int param2);


#endif /* MODULES_COMMANDPARSER_H_ */
