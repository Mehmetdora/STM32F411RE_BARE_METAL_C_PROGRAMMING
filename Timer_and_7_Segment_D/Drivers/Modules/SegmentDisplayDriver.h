/*
 * SegmentDisplayDriver.h
 *
 *  Created on: 18 Şub 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_SEGMENTDISPLAYDRIVER_H_
#define MODULES_SEGMENTDISPLAYDRIVER_H_

#include <stdint.h>
void SegmentDisplayDriver_init(void);
void SegmentDisplayDriver_toggle(void);
void SegmentDisplayDriver_disable(void);
void SegmentDisplayDriver_display(uint32_t value);
void digitalWrite(uint8_t pin, uint8_t val);


#endif /* MODULES_SEGMENTDISPLAYDRIVER_H_ */
