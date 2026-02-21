/*
 * SysTickDriver.h
 *
 *  Created on: 16 Şub 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_SYSTICKDRIVER_H_
#define MODULES_SYSTICKDRIVER_H_

extern volatile uint32_t systick_counter;	// main ve interrupt dosyalarından kullanılacak global sayaç değişkeni
// Tüm dosyalarda aynı değişkeni kullanarak tek değişken üzerinden sayacın değerini kontrol edebilmek için extern ifadesi kullanılmalı

void SysTickDriver_init(void);
void SysTickDriver_disable(void);


#endif /* MODULES_SYSTICKDRIVER_H_ */
