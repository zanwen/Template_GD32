//
// Created by 86157 on 2024/11/11.
//

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#define HAL_BASIC_TIMER_PRE_PRIORITY 2
#define HAL_BASIC_TIMER_SUB_PRIORITY 0

#include "gd32f4xx.h"
void hal_timer_basic_init(uint32_t timer_periph, uint8_t prescaler, uint16_t target_frequency);
void hal_timer5_overflow_callback();
void hal_timer6_overflow_callback();


#endif //HAL_TIMER_H
