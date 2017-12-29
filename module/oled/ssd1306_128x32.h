#ifndef _SSD1306_128X32_H_
#define _SSD1306_128X32_H_

#include <stdint.h>

void ssd1306_SetPosition(uint8_t x, uint8_t y);

void ssd1306_DrawBlack(void);
void ssd1306_DrawWhite(void);

void ssd1306_DrawPixel6x8(uint8_t x, uint8_t y,uint8_t ch[]);
void ssd1306_DrawPixel8x16(uint8_t x, uint8_t y,uint8_t ch[]);
void ssd1306_DrawPixel8x16_Thin(uint8_t x, uint8_t y, uint8_t ch[]);
void ssd1306_DrawPixel8x16(uint8_t x, uint8_t y, uint8_t ch[]);
void ssd1306_DrawPixel2x24(uint8_t x, uint8_t y, uint8_t ch[]);
void ssd1306_DrawPixel8x36(uint8_t x, uint8_t y, uint8_t ch[]);
void ssd1306_DrawPixel2x32(uint8_t x, uint8_t y, uint8_t ch[]);
void ssd1306_DrawPixe14x38(uint8_t x, uint8_t y, uint8_t ch[]);



void ssd1306_DrawFrameBuffer(uint8_t *fb, uint32_t size, uint8_t x, uint8_t y);
void ssd1306_DrawBitmap(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);

#endif // _SSD1306_128X32_H_
