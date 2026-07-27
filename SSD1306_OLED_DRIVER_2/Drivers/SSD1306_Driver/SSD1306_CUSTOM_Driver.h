/*
 * SSD1306_CUSTOM_Driver.h
 *
 *  Created on: Jul 24, 2026
 *      Author: mehmet_dora
 */

#ifndef SSD1306_DRIVER_SSD1306_CUSTOM_DRIVER_H_
#define SSD1306_DRIVER_SSD1306_CUSTOM_DRIVER_H_

#include "stdint.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"









uint16_t GetStringWidth(char* str, SSD1306_Font_t Font);
void SSD1306_WriteCenteredString(char* str, SSD1306_Font_t Font, SSD1306_COLOR color);
void SSD1306_WriteCenteredStringInRegion(char* str,
		SSD1306_Font_t Font, SSD1306_COLOR color,
		uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void SSD1306_DrawCharClipped(char ch, int16_t x, int16_t y,
		SSD1306_Font_t Font, SSD1306_COLOR color,
		uint8_t x_min, uint8_t x_max, uint8_t y_min, uint8_t y_max);

void SSD1306_DrawStringClipped(char* str, int16_t x, int16_t y,
		SSD1306_Font_t Font, SSD1306_COLOR color,
		uint8_t x_min, uint8_t x_max, uint8_t y_min, uint8_t y_max);

void SSD1306_WriteScrollingStringInRegion(char* str, SSD1306_Font_t Font,
		SSD1306_COLOR color, uint8_t x1, uint8_t y1, uint8_t x2,
		uint8_t y2, uint16_t* scroll_pos, uint8_t scrolling_pixels_count);


void DrawImage(uint8_t x,uint8_t y , uint8_t size_x, uint8_t size_y,const unsigned char* arr , SSD1306_COLOR color, SSD1306_Font_t font);


void DrawLiveGraph(void);






#endif /* SSD1306_DRIVER_SSD1306_CUSTOM_DRIVER_H_ */
