/*
 * I2C_Driver.c
 *
 *  Created on: Apr 6, 2026
 *      Author: mehmet_dora
 */


#include "stm32f4xx.h"
#include "stdint.h"
#include "I2C_Driver.h"




void I2C1_init(void){

	RCC->AHB1ENR |= (1UL << 1);		// gpıob clock enable



	GPIOB->OTYPER |= (1UL << 8);	// open - drain
	GPIOB->OTYPER |= (1UL << 9);

	GPIOB->PUPDR |= (1UL << (2*8)); // pull-up
	GPIOB->PUPDR |= (1UL << (2*9));

	GPIOB->MODER &= ~(3UL << (2*8));
	GPIOB->MODER &= ~(3UL << (2*9));

	GPIOB->MODER |= (2UL << (2*8));		// PB8-9 AF mode
	GPIOB->MODER |= (2UL << (2*9));


	GPIOB->AFR[1] |= (4UL << (4*0));
	GPIOB->AFR[1] |= (4UL << (4*1));






	// I2C init

	RCC->APB1ENR |= (1UL << 21);

	I2C1->CR1 |= (1UL << 15);	// reset
	I2C1->CR1 &= ~(1UL << 15);	// unreset


	I2C1->CR1 &= ~(1UL << 7);	// clock stretching enable

	I2C1->CR1 &= ~(1UL << 6);	// general call enable

	I2C1->CR2 |= (1UL << 11);	// DMA enable

	I2C1->CR2 |= (1UL << 12);	// LAST enable

	I2C1->CR2 |= 42UL;			// set clock frequency -> APB1

	I2C1->CCR = 210UL;			// hesaplanması gerekir

	I2C1->TRISE = 43UL;			// hesaplamak lazım

	I2C1->CR1 |= (1UL << 0);	// I2C enable

}


void I2C1_BurstRead(uint8_t saddr, uint8_t maddr, uint8_t n){


	volatile uint32_t tmp;

	// DMA'yı bu okuma için hazırla — enable etme, ADDR_R adımında edilecek
	DMA1_Stream0->CR  &= ~(1UL << 0);
	while (DMA1_Stream0->CR & (1UL << 0));
	DMA1->LIFCR        |= 0x3FUL;
	DMA1_Stream0->NDTR  = n;

	// Hat meşgulse bekle
	while (I2C1->SR2 & (1UL << 1));

	// START
	I2C1->CR1 |= (1UL << 8);
	while (!(I2C1->SR1 & (1UL << 0)));

	// Slave addr + WRITE
	I2C1->DR = (saddr << 1) & 0xFE;
	while (!(I2C1->SR1 & (1UL << 1)));
	tmp = I2C1->SR2;                    // ADDR flag temizle

	// Register adresi gönder
	I2C1->DR = maddr;
	while (!(I2C1->SR1 & (1UL << 7))); // TxE










	// RESTART , read yapmak için
	I2C1->CR1 |= (1UL << 8);
	while (!(I2C1->SR1 & (1UL << 0)));

	// Slave addr + READ
	I2C1->DR = (saddr << 1) | 0x01;
	while (!(I2C1->SR1 & (1UL << 1)));

	// ACK + LAST
	I2C1->CR1 |= (1UL << 10);
	I2C1->CR2 |= (1UL << 12);

	// ADDR temizle — bu andan itibaren DMA veriyi alır
	tmp = I2C1->SR1;
	tmp = I2C1->SR2;

	// DMA enable — fonksiyon buradan döner, veriyi DMA çeker
	DMA1_Stream0->CR |= (1UL << 0);

	(void)tmp;
	// STOP → DMA TC interrupt'ında üretilecek



}

void I2C1_BurstWrite(uint8_t saddr, uint8_t maddr, uint8_t n, uint8_t* data){

	// Bir slave cihaza belirtilen adresten başlayarak n byte veri yazar.
	// Bir ekrana yada bir sensöre config verilerini göndermek için kullanılabilir.

	volatile int tmp;

	while(I2C1->SR2 & (1UL << 1));
	// Eğer hat busy ise bekle, değilse veri okumaya başla
	// aynı anda 2 master çakışmasın diye







	// generate start
	I2C1->CR1 |= (1UL << 8);
	// Start condition oluşturulur, master biz ise bu start ı gönderebiliriz
	// Bus iletişimi başlar
	while(!(I2C1->SR1 & (1UL << 0)));
	// wait for enable start condition, wait for start bit flag





	// transmit slave adress + write
	I2C1->DR = (saddr << 1);
	// Slave adresi gönderiliyor, write modunda olması gerektiği için de lsb 0 olmalı
	//LSB = 0 → WRITE mode
	// 0. bit write-read modunu belirliyor

	// wait for addr flag set
	while(!(I2C1->SR1 & (1UL << 1)));
	// adres gönderildi mi diye beklenir
	// flag oluşması ile slave cihaz adresi almış olur ve ACK göndermiş olur

	// flag clear
	tmp = I2C1->SR2;
	// ADDR flag temizlemek için SR2 değerinin okunması gerekir





	// wait until trasmitter empty
	while(!(I2C1->SR1 & (1UL << 7)));	// TxE
	// Verinin gönderildiğini ve DR ın boşaldığını bekle

	// send memory address
	I2C1->DR = maddr;
	// Slave cihaza hangi adresi okumak istediğini gönder




	for(int i = 0; i<n; i++){
		// wait until trasmitter empty
		while(!(I2C1->SR1 & (1UL << 7)));	// TxE
		// Verinin gönderildiğini ve DR ın boşaldığını bekle

		// read data
		I2C1->DR = *data++;



	}


	// wait until transfer finished
	while(!(I2C1->SR1 & (1UL << 2)));
	// RXNE yerine BTF(byte transfer finished) ile kontrol edilir


	// Generate stop after data received
	I2C1->CR1 |= (1UL << 9);
	// veri alındıktan sonra hat serbest bırakılır


}

void MPU6050_wakeup(uint8_t mpu_saddr){
	uint16_t PWR_MGMT_1 = 0x6B;
	// uykudan uyandırmak için gönderilecek değer : 0x00
	uint8_t wakeup = 0x00;
	I2C1_BurstWrite(mpu_saddr, PWR_MGMT_1, 1, &wakeup);
}


