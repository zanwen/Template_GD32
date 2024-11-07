//
// Created by 86157 on 2024/11/5.
//

#ifndef INT_LED_H
#define INT_LED_H

#include "gd32f4xx.h"

typedef enum {
    LED1 = 0,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,
    LED_SIZE
} LED_NO;

void int_led_init(void);

void int_led_on(LED_NO led_no);
void int_led_off(LED_NO led_no);
void int_led_on_all();
void int_led_off_all();

void int_led_trotting_horse_lamp(LED_NO start, LED_NO end,
                                 int32_t step, uint32_t delay_ms);

void int_led_waterfall_lamp(LED_NO start, LED_NO end,
                            int32_t step, uint32_t delay_ms);

#endif // INT_LED_H
