/*
 * SSD1306_CUSTOM_Driver.c
 *
 *  Created on: Jul 24, 2026
 *      Author: mehmet_dora
 */


#include "SSD1306_CUSTOM_Driver.h"






// Metnin piksel cinsinden toplam genişliğini hesaplayan fonksiyon
uint16_t GetStringWidth(char* str, SSD1306_Font_t Font) {
    uint16_t total_width = 0;
    while (*str) {
        char ch = *str;
        // Karakterin kütüphanedeki ASCII aralığında olduğundan emin ol
        if (ch >= 32 && ch <= 126) {
            // Eğer orantılı font ise karakterin kendi genişliğini ekle, değilse sabit font genişliğini ekle
            total_width += Font.char_width ? Font.char_width[ch - 32] : Font.width;
        }
        str++;
    }
    return total_width;
}
// Metni ekranda yatay ve dikey olarak tam ortalayan fonksiyon
void SSD1306_WriteCenteredString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color) {
    // 1. Metnin genişliğini ve yüksekliğini al
    uint16_t text_width = GetStringWidth(str, Font);
    uint16_t text_height = Font.height;
    // 2. Ortalanmış X ve Y koordinatlarını hesapla
    int16_t x = (SSD1306_WIDTH - text_width) / 2;
    int16_t y = (SSD1306_HEIGHT - text_height) / 2;
    // Koordinatların negatif olmamasını garanti altına al (ekran dışına taşmaması için)
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    // 3. İmleci konumlandır ve yazıyı yazdır
    ssd1306_SetCursor(x, y);
    ssd1306_WriteString(str, Font, color);
}

void SSD1306_WriteCenteredStringInRegion(char* str, SSD1306_Font_t Font, SSD1306_COLOR color,
                                         uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    // 1. Tanımlanan bölgenin genişlik ve yüksekliğini hesapla
    int16_t region_width = x2 - x1 + 1;
    int16_t region_height = y2 - y1 + 1;
    // Koordinatların ve bölge boyutlarının geçerli olduğundan emin ol
    if (region_width <= 0 || region_height <= 0) return;
    // 2. Metnin boyutlarını hesapla
    uint16_t text_width = GetStringWidth(str, Font);
    uint16_t text_height = Font.height;
    // 3. Bölge içindeki ortalanmış koordinatları hesapla
    int16_t relative_x = (region_width - text_width) / 2;
    int16_t relative_y = (region_height - text_height) / 2;
    // Eğer metin bölgeden daha büyükse, sola/yukarıya yasla
    if (relative_x < 0) relative_x = 0;
    if (relative_y < 0) relative_y = 0;
    // 4. Mutlak ekran koordinatlarını hesapla
    uint8_t absolute_x = x1 + relative_x;
    uint8_t absolute_y = y1 + relative_y;
    // Ekran sınırlarını aşmasını engelle
    if (absolute_x >= SSD1306_WIDTH) absolute_x = SSD1306_WIDTH - 1;
    if (absolute_y >= SSD1306_HEIGHT) absolute_y = SSD1306_HEIGHT - 1;
    // 5. Yazdır
    ssd1306_SetCursor(absolute_x, absolute_y);
    ssd1306_WriteString(str, Font, color);
}








void SSD1306_DrawCharClipped(char ch, int16_t x, int16_t y, SSD1306_Font_t Font, SSD1306_COLOR color,
                             uint8_t x_min, uint8_t x_max, uint8_t y_min, uint8_t y_max) {
    uint32_t i, b, j;
    if (ch < 32 || ch > 126) return;

    const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;

    for(i = 0; i < Font.height; i++) {
        b = Font.data[(ch - 32) * Font.height + i];
        for(j = 0; j < char_width; j++) {
            int16_t pixel_x = x + j;
            int16_t pixel_y = y + i;

            // Eğer piksel sınırların içindeyse ekrana çiz
            if (pixel_x >= x_min && pixel_x <= x_max && pixel_y >= y_min && pixel_y <= y_max) {
                if((b << j) & 0x8000)  {
                    ssd1306_DrawPixel(pixel_x, pixel_y, color);
                } else {
                    ssd1306_DrawPixel(pixel_x, pixel_y, (SSD1306_COLOR)!color);
                }
            }
        }
    }
}
// 2. Metni sadece belirtilen sınırlar içine çizen yardımcı fonksiyon
void SSD1306_DrawStringClipped(char* str, int16_t x, int16_t y, SSD1306_Font_t Font, SSD1306_COLOR color,
                               uint8_t x_min, uint8_t x_max, uint8_t y_min, uint8_t y_max) {
    int16_t current_x = x;
    while (*str) {
        char ch = *str;
        if (ch >= 32 && ch <= 126) {
            const uint8_t char_width = Font.char_width ? Font.char_width[ch-32] : Font.width;

            // Sadece görünür bölgedeki karakterleri çizerek işlemciyi yormuyoruz
            if (current_x + char_width >= x_min && current_x <= x_max) {
                SSD1306_DrawCharClipped(ch, current_x, y, Font, color, x_min, x_max, y_min, y_max);
            }
            current_x += char_width;
        }
        str++;
    }
}
/**
 * @brief  Belirtilen bölgeye sığmayan metni sola doğru kaydırarak yazar.
 * @param  str: Yazdırılacak metin
 * @param  Font: Kullanılacak yazı tipi
 * @param  color: Metin rengi
 * @param  x1, y1: Bölgenin sol üst koordinatı
 * @param  x2, y2: Bölgenin sağ alt koordinatı
 * @param  scroll_pos: Kayma miktarını tutan kalıcı (static) değişkenin adresi
 */
void SSD1306_WriteScrollingStringInRegion(char* str, SSD1306_Font_t Font, SSD1306_COLOR color,
                                          uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                                          uint16_t* scroll_pos, uint8_t scrolling_pixels_count) {
    int16_t region_width = x2 - x1 + 1;
    int16_t region_height = y2 - y1 + 1;

    if (region_width <= 0 || region_height <= 0) return;
    uint16_t text_width = GetStringWidth(str, Font);

    // Metin dikeyde ortalansın
    int16_t y_pos = y1 + (region_height - Font.height) / 2;
    if (y_pos < 0) y_pos = 0;
    // EĞER METİN BÖLGEYE SIĞIYORSA: Kaydırma yapma, doğrudan ortala
    if (text_width <= region_width) {
        int16_t x_pos = x1 + (region_width - text_width) / 2;
        SSD1306_DrawStringClipped(str, x_pos, y_pos, Font, color, x1, x2, y1, y2);
        *scroll_pos = 0; // Kaydırmayı sıfırla
        return;
    }
    // EĞER METİN SIĞMIYORSA: Sola kaydırarak çiz
    // Başlangıç X koordinatını kaydırma miktarına göre sola kaydırıyoruz
    int16_t x_start = x1 - *scroll_pos;

    SSD1306_DrawStringClipped(str, x_start, y_pos, Font, color, x1, x2, y1, y2);
    // Kaydırma koordinatını güncelle (Hızı ayarlamak için)
    static uint8_t pause_ticks = 0;

    // Yazı ilk başladığında kullanıcının okuyabilmesi için 15 tick (kare) bekletelim
    if (*scroll_pos == 0 && pause_ticks < 15) {
        pause_ticks++;
        return;
    }
    pause_ticks = 0;


    // 1 piksel sola kaydır -> burada daha hızlı kaydırma ayarlanabilir
    *scroll_pos += (scrolling_pixels_count);
    // Yazı tamamen ekrandan çıktıktan sonra (veya son harf bölge sınırına geldiğinde) başa sar
    // Burada metinden sonra 30 piksellik bir boşluk bırakıp öyle başa sarıyoruz
    if (*scroll_pos > text_width) {
        *scroll_pos = 0;
    }
}














// ***************  Resim çizdirme *******************



void DrawImage(uint8_t x,uint8_t y , uint8_t size_x, uint8_t size_y,const unsigned char* arr , SSD1306_COLOR color, SSD1306_Font_t font) {
    ssd1306_Fill(Black);

    // Sağ üst köşeye (x=110, y=2) 16x16 boyutundaki Wi-Fi simgesini beyaz renk olarak çizer
    ssd1306_DrawBitmap(x,y, arr, size_x,size_y, color);

    //ssd1306_SetCursor(5, 5);
    //ssd1306_WriteString("Baglandi", font, color);

    ssd1306_UpdateScreen();
}





// ************* Grafik Çizdirme *********************


void DrawLiveGraph(void) {
    ssd1306_Fill(Black);

    // Grafik sınır çizgilerini çizelim
    ssd1306_Line(15, 5, 15, 55, White);  // Y ekseni
    ssd1306_Line(15, 55, 120, 55, White); // X ekseni

    // Grafik üzerindeki veri noktaları (X, Y koordinat çiftleri)
    SSD1306_VERTEX graph_data[] = {
        {20, 24},  // 1. nokta
        {35, 45},  // 2. nokta
        {50, 33},  // 3. nokta
        {65, 25},  // 4. nokta (Zirve noktası)
        {80, 53},  // 5. nokta
        {95, 16},  // 6. nokta
        {110, 41}  // 7. nokta
    };

    // Noktaları birleştirerek grafiği çiz
    uint16_t point_count = sizeof(graph_data) / sizeof(graph_data[0]);
    ssd1306_Polyline(graph_data, point_count, White);

    ssd1306_UpdateScreen();
}



