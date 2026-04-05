/*
 * I2C_DMA_Driver.c
 *
 *  Created on: Apr 4, 2026
 *      Author: mehmet_dora
 */

#include "I2C_DMA_Driver.h"
#include "stdint.h"
#include "stm32f4xx.h"

volatile uint8_t  RAW_BUFFER[RAW_BUFFER_SIZE];
volatile Sample_t CIRC_BUFFER[CIRC_SIZE];

volatile uint8_t uart_tx_ready = 0;
volatile uint8_t circ_index   = 0;
static uint8_t hpos = 0;

// moving average geçmişi
static int16_t h_ax[4], h_ay[4], h_az[4];
static int16_t h_gx[4], h_gy[4], h_gz[4];





static int16_t mavg(int16_t* h, int16_t v) {
    h[hpos & 3] = v;
    return (h[0] + h[1] + h[2] + h[3]) >> 2;
}

void I2C_DMA_init(void){

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


void I2C1_rx_DMA_stream0_init(void){


	// stream 0 , channel 1 -> ı2c1 rx


	RCC->AHB1ENR |= (1UL << 21);		// DMA1 clock enable

	DMA1_Stream0->CR &= ~(1UL << 0);	// Önce disable et, sonra enable olmasını bekle
	while((DMA1_Stream0->CR & (1UL << 0)) != 0){};

	DMA1_Stream0->CR |= (1UL << 25);	// chanel 1 selected
	DMA1_Stream0->CR |= (1UL << 10);	// memory increment enable
	DMA1_Stream0->CR &= ~(3UL << 6);	// peripheral to memory


	DMA1_Stream0->CR &= ~(1UL << 9);	// DMA1 peripheral address sabit , 	sabit çünkü peripheral hep aynı adresten veri okuyor veya alıyor
	DMA1_Stream0->CR &= ~(3UL << 11);	// DMA1 peripheral size 8 bit,		I2C 8 bit olarak ayarlandığı için 8 bit seçildi
	DMA1_Stream0->CR &= ~(3UL << 13);	// DMA1 memory size 8 bit
	DMA1_Stream0->CR |= (2UL << 16);	// DMA1 priority level high

	DMA1_Stream0->CR |= (1UL << 4);		// Transfer complete interrupt enable
	DMA1_Stream0->FCR &= ~(1UL << 2);	// direct mode enable



	DMA1_Stream0->PAR = (uint32_t)&I2C1->DR;		// Peripheral adress
	DMA1_Stream0->M0AR = (uint32_t)RAW_BUFFER;		// Memory adress

	NVIC_SetPriority(DMA1_Stream0_IRQn, 0);
	NVIC_EnableIRQ(DMA1_Stream0_IRQn);

}



void I2C1_BurstRead_Blocking(uint8_t saddr, uint8_t maddr, uint8_t n) {

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



void DMA1_Stream0_IRQHandler(void) {
    if (!(DMA1->LISR & (1UL << 5))) return;
    DMA1->LIFCR |= (1UL << 5);

    I2C1->CR1 |= (1UL << 9);           // STOP

    int16_t raw_ax = (int16_t)((RAW_BUFFER[0]  << 8) | RAW_BUFFER[1]);
    int16_t raw_ay = (int16_t)((RAW_BUFFER[2]  << 8) | RAW_BUFFER[3]);
    int16_t raw_az = (int16_t)((RAW_BUFFER[4]  << 8) | RAW_BUFFER[5]);
    int16_t raw_gx = (int16_t)((RAW_BUFFER[8]  << 8) | RAW_BUFFER[9]);
    int16_t raw_gy = (int16_t)((RAW_BUFFER[10] << 8) | RAW_BUFFER[11]);
    int16_t raw_gz = (int16_t)((RAW_BUFFER[12] << 8) | RAW_BUFFER[13]);

    uint8_t idx = circ_index % CIRC_SIZE;
    CIRC_BUFFER[idx].ax = mavg(h_ax, raw_ax);
    CIRC_BUFFER[idx].ay = mavg(h_ay, raw_ay);
    CIRC_BUFFER[idx].az = mavg(h_az, raw_az);
    CIRC_BUFFER[idx].gx = mavg(h_gx, raw_gx);
    CIRC_BUFFER[idx].gy = mavg(h_gy, raw_gy);
    CIRC_BUFFER[idx].gz = mavg(h_gz, raw_gz);

    circ_index++;
    hpos++;
}




void DMA_i2c_rx_start(void){
	DMA1_Stream0->CR |= (1UL << 0);	// DMA1 stream enable
}













