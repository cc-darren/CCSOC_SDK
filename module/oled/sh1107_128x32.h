#ifndef _SH1107_128X32_H_
#define _SH1107_128X32_H_

#include <stdint.h>

void sh1107_SetPosition(uint8_t x, uint8_t y);

void sh1107_DrawBlack(void);
void sh1107_DrawWhite(void);

void sh1107_DrawPixel6x8(uint8_t x, uint8_t y,uint8_t ch[]);
void sh1107_DrawPixel8x16(uint8_t x, uint8_t y,uint8_t ch[]);
void sh1107_DrawPixel2x24(uint8_t x, uint8_t y, uint8_t ch[]);
void sh1107_DrawPixel8x36(uint8_t x, uint8_t y, uint8_t ch[]);
void sh1107_DrawPixel8x16_Thin(uint8_t x, uint8_t y, uint8_t ch[]);


void sh1107_DrawFrameBuffer(uint8_t *fb, uint32_t size, uint8_t x, uint8_t y);
void sh1107_DrawBitmap(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[]);

#endif // _SSD1306_128X32_H_
