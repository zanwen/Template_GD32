//
// Created by 86157 on 2024/11/6.
//

#ifndef INT_KEY_H
#define INT_KEY_H

#include "com_util.h"
#include "gd32f4xx.h"

typedef enum {
    KEY1 = 0,
    KEY2,
    KEY3,
    KEY4,
    KEY_SIZE
} KEY_NO;

typedef void (*key_callback_t)(KEY_NO event_key);

void int_key_init(void);
void int_key_scan(void);
void key_up_callback(KEY_NO key_no);
void key_down_callback(KEY_NO key_no);

#endif//INT_KEY_H
