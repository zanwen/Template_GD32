//
// Created by 86157 on 2024/11/6.
//

#ifndef DRI_USART_H
#define DRI_USART_H

#include "gd32f4xx_usart.h"
#include <stdbool.h>
#include "com_typedef.h"

void hal_usart0_init(void);

void hal_usart0_send_byte(uint8_t data);
void hal_usart0_send_bytes(uint8_t data[], uint32_t len);
void hal_usart0_send_str(char *str);

void hal_usart0_handle_irq(void);
void hal_usart0_read_complete_callback(callback_t callback);
bool hal_usart0_isreadable(void);
uint8_t hal_usart0_get_byte(void);
uint16_t hal_usart0_read(uint8_t *buf, uint16_t bufsize);
uint16_t hal_usart0_get_str(uint8_t *buf, uint16_t bufsize);


#endif//DRI_USART_H
