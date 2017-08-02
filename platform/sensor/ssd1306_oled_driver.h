


#ifndef __MCUBE_CALLSENSE_H__
#define __MCUBE_CALLSENSE_H__
#include <stdbool.h>
#include <stdint.h>

#define   DISABLE_CHINESESTRING       0



uint8_t lcdGetStringLength(const char *pcStr);
void OLED_Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_P16x16Ch(unsigned char x, unsigned char y,unsigned char  N);
void OLED_P8x16Str(unsigned char x, unsigned y,unsigned char ch[]);
void OLED_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);
void OLED_Display_Black(void);
void OLED_Display_White(void);
void OLED_Display_Buffer(uint8_t* buf, uint32_t size, uint8_t x, uint8_t y);
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
void OLED_Enable_SleepMode(char enable);




#if DISABLE_CHINESESTRING
void RunningChineseString16(char* pstr, unsigned int delayms);
void RunningString16(char* pstr, unsigned int delayms);
void LCD_P16x16Ch(unsigned char x, unsigned char y, unsigned char N);
void LCD_P16x16ChString(unsigned char x, unsigned char y, unsigned char N, char* pStr);
void OLED_BufferPrintString(char *pcBuffer, const char *pcStr, uint8_t ui8X);
#endif

#endif
