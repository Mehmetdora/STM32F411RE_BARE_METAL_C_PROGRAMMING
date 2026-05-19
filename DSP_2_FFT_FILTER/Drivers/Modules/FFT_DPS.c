/*
 * FFT_DPS.c
 *
 *  Created on: May 17, 2026
 *      Author: mehmet_dora
 */


#include "FFT_DSP.h"
#include "arm_math.h"
#include <string.h>

// --- Statik tamponlar (stack'e koyma, büyükler) ---
static float32_t fft_input[FFT_SIZE];        // FFT girişi (float)
static float32_t fft_output[FFT_SIZE];       // FFT çıkışı (karmaşık)
static float32_t fft_magnitude[FFT_SIZE/2];  // Güç spektrumu

// CMSIS-DSP FFT nesnesi
static arm_rfft_fast_instance_f32 fft_handler;
static uint8_t fft_initialized = 0;

// --- Hanning penceresi katsayıları ---
static float32_t hanning_window[FFT_SIZE];

static void init_hanning_window(void) {
    for(int i = 0; i < FFT_SIZE; i++) {
        // w(n) = 0.5 * (1 - cos(2π*n / (N-1)))
        hanning_window[i] = 0.5f * (1.0f - arm_cos_f32(
            2.0f * PI * i / (FFT_SIZE - 1)
        ));
    }
}

// --- Bölgedeki maksimum gücü bul ---
static float32_t get_max_power(uint32_t bin_low, uint32_t bin_high) {
    float32_t max_val = 0.0f;
    for(uint32_t i = bin_low; i <= bin_high; i++) {
        if(fft_magnitude[i] > max_val) {
            max_val = fft_magnitude[i];
        }
    }
    return max_val;
}

// --- Ana fonksiyon: ADC tamponunu al, tespit sonucu döndür ---
DetectionResult fft_process(uint16_t* adc_buffer) {

    // İlk çağrıda başlat
    if(!fft_initialized) {
        arm_rfft_fast_init_f32(&fft_handler, FFT_SIZE);
        init_hanning_window();
        fft_initialized = 1;
    }

    // 1. ADC → float dönüşümü + DC offset temizle + Hanning uygula
    for(int i = 0; i < FFT_SIZE; i++) {
        float32_t sample = (float32_t)adc_buffer[i] - ADC_DC_OFFSET;
        fft_input[i] = sample * hanning_window[i];
    }

    // 2. FFT hesapla (gerçek → karmaşık)
    arm_rfft_fast_f32(&fft_handler, fft_input, fft_output, 0);

    // 3. Magnitude (güç) hesapla: sqrt(re² + im²)
    arm_cmplx_mag_f32(fft_output, fft_magnitude, FFT_SIZE / 2);

    // 4. bin[0] = DC, yok say
    fft_magnitude[0] = 0.0f;

    // 5. İlgili bölgelerdeki max gücü al
    float32_t moto_power = get_max_power(MOTO_BIN_LOW, MOTO_BIN_HIGH);
    float32_t yaya_power = get_max_power(YAYA_BIN_LOW, YAYA_BIN_HIGH);

    debug_moto_power = moto_power;
	debug_yaya_power = yaya_power;

    // 6. Karar ver
    if(moto_power > MOTO_POWER_THRESHOLD) {
        return DETECT_MOTORSIKLET;
    } else if(yaya_power > YAYA_POWER_THRESHOLD) {
        return DETECT_YAYA;
    }




    return DETECT_NOTHING;
}
