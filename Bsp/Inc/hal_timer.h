//
// Created by 86157 on 2024/11/11.
//

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#define HAL_BASIC_TIMER_PRE_PRIORITY 2
#define HAL_BASIC_TIMER_SUB_PRIORITY 0

#define HAL_TIMER_PRESCALER 10

#include "gd32f4xx.h"
#include "stdbool.h"

void hal_timer_basic_start(uint32_t timer_periph, uint16_t freq);

void hal_timer5_update_callback();

void hal_timer6_update_callback();

void hal_timer_init(rcu_periph_enum periph_clk, uint32_t timer_periph, uint16_t freq);

void hal_timer_pwm_channel_enable(uint32_t timer_periph, uint16_t channel, bool complementary);

void hal_timer_pwm_set_duty_cycle(uint32_t timer_periph, uint16_t channel,
                                  uint16_t freq, uint16_t duty_cycle);

void hal_timer_pwm_set_freq(uint32_t timer_periph, uint16_t freq);

void hal_timer_disable(uint32_t timer_periph);

void hal_timer_enable(uint32_t timer_periph);

#endif //HAL_TIMER_H
