//
// Created by 86157 on 2024/11/6.
//

#ifndef RETARGET_H
#define RETARGET_H


#include "gd32f4xx_usart.h"
#include <stdio.h>

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#ifdef __GNUC__
#define GETCHAR_PROTOTYPE int __io_getchar(void)
#else
#define GETCHAR_PROTOTYPE int fgetc(FILE *f)
#endif /* __GNUC__ */

/**
  * @brief 注册重定向串口
  * @param usart_periph 需要重定向输入输出的串口
  */
void retarget_init(uint32_t usart_periph);

#endif//RETARGET_H
