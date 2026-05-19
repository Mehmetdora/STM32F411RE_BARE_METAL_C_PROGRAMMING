/*
 * FFT_DSP.h
 *
 *  Created on: May 17, 2026
 *      Author: mehmet_dora
 */

#ifndef MODULES_FFT_DSP_H_
#define MODULES_FFT_DSP_H_



#include "arm_math.h"
#include "stdint.h"

// --- Parametreler ---
#define FFT_SIZE        256       // Pencere boyutu
#define SAMPLE_RATE     200.0f    // Hz — TIM2 ile ayarladığın hız
#define ADC_DC_OFFSET   2030.0f   // Hareketsizken ortalama ADC değerin

// Frekans bin hesabı: bin_no = frekans * FFT_SIZE / SAMPLE_RATE
#define FREQ_TO_BIN(f)  ((uint32_t)((f) * FFT_SIZE / SAMPLE_RATE))

// Motorsiklet frekans bölgesi (6–18 Hz)
#define MOTO_BIN_LOW    FREQ_TO_BIN(8.0f)    // bin 8
#define MOTO_BIN_HIGH   FREQ_TO_BIN(23.0f)   // bin 23

// Yaya frekans bölgesi (1–4 Hz)
#define YAYA_BIN_LOW    FREQ_TO_BIN(1.0f)    // bin 1
#define YAYA_BIN_HIGH   FREQ_TO_BIN(5.0f)    // bin 5

// Eşik değerleri — sahada ayarlarsın
#define MOTO_POWER_THRESHOLD   500.0f
#define YAYA_POWER_THRESHOLD   300.0f

// Tespit sonucu
typedef enum {
    DETECT_NOTHING,
    DETECT_YAYA,
    DETECT_MOTORSIKLET
} DetectionResult;




extern volatile float debug_moto_power;
extern volatile float debug_yaya_power;


// Dışarıya açık fonksiyon — main.c bu fonksiyonu çağırır
DetectionResult fft_process(uint16_t* adc_buffer);


#endif /* MODULES_FFT_DSP_H_ */
