//
// Created by 86157 on 2024/11/8.
//

#ifndef COM_TYPEDEF_H
#define COM_TYPEDEF_H
#include "gd32f4xx.h"

typedef void (*callback_t)(void);
typedef void(*usart_read_complete_callabck_t)(uint8_t *buf);

#endif//COM_TYPEDEF_H
