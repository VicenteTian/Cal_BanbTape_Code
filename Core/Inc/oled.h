/*
 * @Author: Tian Wei
 * @Date: 2022-05-18 13:41:16
 * @Description: 
 * @Version: V1.0
 */
#ifndef __OLED_H
#define __OLED_H
#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "stdlib.h"
#include "gpio.h"
#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x02
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define Oled_Enable 1
#define OLED_CMD 0  
#define OLED_DATA 1 
#define OLED_ADDR 0x7A  //0x78
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
void OLED_Fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *p, uint8_t Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1,const unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
#endif
