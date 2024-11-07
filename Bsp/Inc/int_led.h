//
// Created by 86157 on 2024/11/5.
//

#ifndef INT_LED_H
#define INT_LED_H

#include "gd32f4xx.h"

#define LED_ON RESET
#define LED_OFF SET

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
} LED_INDEX;

void int_led_init(void);

void int_led_on(LED_INDEX led_index, uint32_t delay_ms);
void int_led_off(LED_INDEX led_index, uint32_t delay_ms);
void int_led_on_all(uint32_t delay_ms);
void int_led_off_all(uint32_t delay_ms);
void int_led_turn(LED_INDEX led_index, bit_status led_status, uint32_t delay_ms);

/**
 *
 * @param leds
 * @param len
 * @param state LED_ON, LED_OFF
 * @param delay_ms
 */
void int_led_control(LED_INDEX leds[], uint8_t len, bit_status state, uint32_t delay_ms);

void int_led_trotting_horse_lamp(LED_INDEX start, LED_INDEX end,
                                 int32_t step, uint32_t delay_ms);

void int_led_waterfall_lamp(LED_INDEX start, LED_INDEX end,
                            int32_t step, uint32_t delay_ms);

#endif// INT_LED_H
