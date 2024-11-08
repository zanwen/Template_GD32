//
// Created by 86157 on 2024/11/6.
//
#include "../Inc/retarget.h"

/* 定义USART端口，用于注册重定向的串口(也可以用UART，根据实际情况来改写) */
static uint32_t sg_usart_periph;

/**
  * @brief 注册重定向串口
  * @param usart_periph 需要重定向输入输出的串口
  */
void retarget_init(uint32_t usart_periph) {
    /* 注册串口 */
    sg_usart_periph = usart_periph;

    /* Disable I/O buffering for STDOUT stream, so that
      * chars are sent out as soon as they are printed. */
    setvbuf(stdout, NULL, _IONBF, 0);
    /* Disable I/O buffering for STDIN stream, so that
      * chars are received in as soon as they are scanned. */
    setvbuf(stdin, NULL, _IONBF, 0);

}

/**
   * @brief  Retargets the C library printf function to the USART.
   * @param  None
   * @retval None
   */
PUTCHAR_PROTOTYPE {
    /* 发送一个字节数据到串口 */
    /* 很简单，直接调用库函数中的 串口发送数据函数 */
    usart_data_transmit(sg_usart_periph, (uint8_t) ch);

    /*等待发送完毕*/
    while (usart_flag_get(sg_usart_periph, USART_FLAG_TBE) == RESET);
    /* 返回发送的字符 */
    return ch;

}

/**
   * @brief  Retargets the C library scanf, getchar function to the USART.
   * @param  None
   * @retval None
   */
GETCHAR_PROTOTYPE {
    // TODO 暂不支持与USART读中断同时使用
    while (usart_flag_get(sg_usart_periph, USART_FLAG_RBNE) == RESET);
    return (int) usart_data_receive(sg_usart_periph);
}