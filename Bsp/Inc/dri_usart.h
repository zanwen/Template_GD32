//
// Created by 86157 on 2024/11/6.
//

#ifndef DRI_USART_H
#define DRI_USART_H

#include "gd32f4xx_usart.h"

void dri_USART0_init(void);

void dri_USART_send_byte(uint32_t usart_periph, uint8_t data);
void dri_USART_send_bytes(uint32_t usart_periph, uint8_t data[], uint32_t len);
void dri_USART_send_str(uint32_t usart_periph, char *str);

#endif//DRI_USART_H
