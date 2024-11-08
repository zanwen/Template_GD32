//
// Created by 86157 on 2024/11/6.
//

#ifndef DRI_USART_H
#define DRI_USART_H

#include "gd32f4xx_usart.h"
#include <stdbool.h>
#include "com_typedef.h"

void dri_usart0_init(void);

void dri_usart0_send_byte(uint8_t data);
void dri_usart0_send_bytes(uint8_t data[], uint32_t len);
void dri_usart0_send_str(char *str);

void dri_usart0_handle_irq(void);
void dri_usart0_read_complete_callabck(callback_t callback);
bool dri_usart0_isreadable(void);
uint8_t dri_usart0_get_char(void);
uint16_t dri_usart0_read(uint8_t *buf, uint16_t bufsize);
uint16_t dri_usart0_get_str(uint8_t *buf, uint16_t bufsize);


#endif//DRI_USART_H
