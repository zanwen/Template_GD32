#ifndef __INT_I2C_OLED_H
#define __INT_I2C_OLED_H 

#include "stdlib.h"
#include "com_typedef.h"
#include "int_i2c_oledfont.h"


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void I2C_OLED_ColorTurn(u8 i);
void I2C_OLED_DisplayTurn(u8 i);
void I2C_OLED_WR_Byte(u8 dat,u8 mode);
void I2C_OLED_DisPlay_On(void);
void I2C_OLED_DisPlay_Off(void);
void I2C_OLED_Refresh(void);
void I2C_OLED_Clear(void);
void I2C_OLED_DrawPoint(u8 x,u8 y,u8 t);
void I2C_OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode);
void I2C_OLED_DrawCircle(u8 x,u8 y,u8 r);
void I2C_OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode);
void I2C_OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode);
void I2C_OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode);
void I2C_OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode);
void I2C_OLED_ScrollDisplay(u8 num,u8 space,u8 mode);
void I2C_OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode);
void I2C_OLED_Init(void);

#endif

