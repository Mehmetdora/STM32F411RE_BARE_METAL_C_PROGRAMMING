/*
 * I2C_DMA_Driver.c
 *
 *  Created on: Apr 4, 2026
 *      Author: mehmet_dora
 */

#include "I2C_DMA_Driver.h"
#include "TimerDriver.h"

#include "stdint.h"
#include "stm32f4xx.h"

volatile uint8_t  RAW_BUFFER[RAW_BUFFER_SIZE];
volatile uint8_t CIRC_BUFFER[CIRC_SIZE * 12];	// sensör okuma sayısı * eksen sayısı * 2 byte her eksen için

volatile uint8_t uart_tx_ready = 0;
volatile uint16_t circ_index   = 0;
static uint8_t hpos = 0;

// moving average geçmişi
static int16_t h_ax[4], h_ay[4], h_az[4];
static int16_t h_gx[4], h_gy[4], h_gz[4];




static int16_t mavg(int16_t* h, int16_t v) {
    h[hpos % 4] = v;
    return (h[0] + h[1] + h[2] + h[3]) >> 2;
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



void DMA1_Stream0_IRQHandler(void) {
    if (!(DMA1->LISR & (1UL << 5))) return;
    DMA1->LIFCR |= (1UL << 5);

    I2C1->CR1 |= (1UL << 9);           // STOP


    /*
     * Burada sensörden 14 byte lık veri okunuyor, bu verilerden ilk 6 byte accel(x-y-z)
     * sonraki 2 byte temp , en sondaki 6 byte gyro(x-y-z) veirleri alınıyor.
     *
     * Bu verilerdeki sadece gyro ve accel alınırken temp de arada mecburen okunacağı için
     * RAW_BUFFER a tüm veriler DMA tarafından yazılıyor. Yani RAw buffer 14 byte olsada bize
     * gereken 12 byte lık kısım işleniyor, bufferdaki  6-7. indexdekiler atlanıyor kasten.
     *
     *
     * Burada bufferdaki eksen verileri 2 ayrı registerda okunduğundan bu eksen verilerinin
     * birleştirilerek o eksen için tek bir değer şeklinde kullanabilmek için or'lanır.
     * bu sırada uint16 ya da dönüştürüleceğini belirtmek lazım
     */

    int16_t raw_ax = (int16_t)((RAW_BUFFER[0]  << 8) | RAW_BUFFER[1]);
    int16_t raw_ay = (int16_t)((RAW_BUFFER[2]  << 8) | RAW_BUFFER[3]);
    int16_t raw_az = (int16_t)((RAW_BUFFER[4]  << 8) | RAW_BUFFER[5]);
    int16_t raw_gx = (int16_t)((RAW_BUFFER[8]  << 8) | RAW_BUFFER[9]);
    int16_t raw_gy = (int16_t)((RAW_BUFFER[10] << 8) | RAW_BUFFER[11]);
    int16_t raw_gz = (int16_t)((RAW_BUFFER[12] << 8) | RAW_BUFFER[13]);


    /*
     * buradaki circ_index her saniyede bir artıyor, size ile % alınarak buffer daki
     * tutulacağı index belirlenmiş oluyor
     *
     * 30 sn de bir circ_index sıfırlanıyor, yani veri buffer üzerine yazılmaya başlanıyor
     */

    uint16_t temp;

    temp = mavg(h_ax, raw_ax);
    CIRC_BUFFER[circ_index+0] =  temp & 0xFF;				// 16 byte ın ilk 8 byte ını al
    CIRC_BUFFER[circ_index+1] =  (temp >> 8) & 0xFF;		// 16 byte ın son 8 byte ını al

    temp = mavg(h_ay, raw_ay);
	CIRC_BUFFER[circ_index+2] =  temp & 0xFF;
	CIRC_BUFFER[circ_index+3] =  (temp >> 8) & 0xFF;

	temp = mavg(h_az, raw_az);
	CIRC_BUFFER[circ_index+4] =  temp & 0xFF;
	CIRC_BUFFER[circ_index+5] =  (temp >> 8) & 0xFF;

	temp = mavg(h_gx, raw_gx);
	CIRC_BUFFER[circ_index+6] =  temp & 0xFF;
	CIRC_BUFFER[circ_index+7] =  (temp >> 8) & 0xFF;

	temp = mavg(h_gy, raw_gy);
	CIRC_BUFFER[circ_index+8] =  temp & 0xFF;
	CIRC_BUFFER[circ_index+9] =  (temp >> 8) & 0xFF;

	temp = mavg(h_gz, raw_gz);
	CIRC_BUFFER[circ_index+10] =  temp & 0xFF;
	CIRC_BUFFER[circ_index+11] =  (temp >> 8) & 0xFF;


    circ_index += 12;
    hpos++;


    /*
     * 30 örneklik veri toplandıktan sonra göndermeye başlamak için son sensör
     * okuması da tamamlandıktan sonra asıl gönderme işlemi yapılması gerekiyor burada
     */
    if(counter >= 30){
		counter = 0;
		uart_tx_ready = 1;
	}
}




void DMA_i2c_rx_start(void){
	DMA1_Stream0->CR |= (1UL << 0);	// DMA1 stream enable
}













