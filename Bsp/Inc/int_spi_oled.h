#ifndef __SPI_OLED_H
#define __SPI_OLED_H 

#include "gd32f4xx.h"
#include "com_typedef.h"
#include "systick.h"
//SCL=SCLK 
//SDA=MOSI
//DC=DC
//CS=CS1
//FS0=MOSI
//CS2=CS2
//-----------------OLED端口定义---------------- 



#define SPI_OLED_DC_Clr()  gpio_bit_reset(GPIOA,GPIO_PIN_2)//DC
#define SPI_OLED_DC_Set()  gpio_bit_set(GPIOA,GPIO_PIN_2)
 		     
#define SPI_OLED_CS_Clr()  gpio_bit_reset(GPIOA,GPIO_PIN_3)//CS1
#define SPI_OLED_CS_Set()  gpio_bit_set(GPIOA,GPIO_PIN_3)

#define SPI_OLED_ROM_CS_Clr()  gpio_bit_reset(GPIOC,GPIO_PIN_5)//CS2
#define SPI_OLED_ROM_CS_Set()  gpio_bit_set(GPIOC,GPIO_PIN_5)

#define SPI_OLED_CMD  0	//写命令
#define SPI_OLED_DATA 1	//写数据

void SPI_OLED_ColorTurn(u8 i);
void SPI_OLED_DisplayTurn(u8 i);
void SPI_OLED_WR_Byte(u8 dat,u8 cmd);
void SPI_OLED_Clear(void);
void SPI_OLED_address(u8 x,u8 y);
void SPI_OLED_Display_128x64(u8 *dp);
void SPI_OLED_Display_16x16(u8 x,u8 y,u8 *dp);
void SPI_OLED_Display_8x16(u8 x,u8 y,u8 *dp);
void SPI_OLED_Display_5x7(u8 x,u8 y,u8 *dp);
void Send_Command_to_ROM(u8 dat);
u8 Get_data_from_ROM(void);
void SPI_OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen);
void SPI_OLED_Display_GB2312_string(u8 x,u8 y,u8 *text);
void SPI_OLED_Display_string_5x7(u8 x,u8 y,u8 *text);
void SPI_OLED_ShowNum(u8 x,u8 y,float num,u8 len);
void SPI_OLED_Init(void);
#endif

