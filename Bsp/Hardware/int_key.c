//
// Created by 86157 on 2024/11/6.
//
#include "../Inc/int_key.h"
#include "../Inc/com_util.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "systick.h"

#define KEY1_GPIO_PORT GPIOC
#define KEY1_GPIO_PIN GPIO_PIN_0

#define KEY2_GPIO_PORT GPIOC
#define KEY2_GPIO_PIN GPIO_PIN_1

#define KEY3_GPIO_PORT GPIOC
#define KEY3_GPIO_PIN GPIO_PIN_2

#define KEY4_GPIO_PORT GPIOC
#define KEY4_GPIO_PIN GPIO_PIN_3

#define PRESSED RESET
#define RELEASED SET

#define DEBOUNCE_DELAY_MS 10// 定义消抖延时，典型值为 10~20ms

static void init_key_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin);

static gpio_port_pin_t key_gpio[KEY_SIZE] = {
        {KEY1_GPIO_PORT, KEY1_GPIO_PIN, RCU_GPIOC},
        {KEY2_GPIO_PORT, KEY2_GPIO_PIN, RCU_GPIOC},
        {KEY3_GPIO_PORT, KEY3_GPIO_PIN, RCU_GPIOC},
        {KEY4_GPIO_PORT, KEY4_GPIO_PIN, RCU_GPIOC}};

static key_callback_t key_pressed_callbacks[KEY_SIZE];
static key_callback_t key_released_callbacks[KEY_SIZE];
static FlagStatus key_pre_state[KEY_SIZE];

void int_key_init(void) {
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        init_key_gpio(key_gpio[key_no].rcu_gpio, key_gpio[key_no].port, key_gpio[key_no].pin);
    }
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        key_pressed_callbacks[key_no] = NULL;
        key_released_callbacks[key_no] = NULL;
        key_pre_state[key_no] = RELEASED;
    }
}
void int_key_scan(void) {
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        FlagStatus current_state = gpio_input_bit_get(key_gpio[key_no].port, key_gpio[key_no].pin);
        if (current_state != key_pre_state[key_no]) {
            // double check for jitter
            delay_1ms(DEBOUNCE_DELAY_MS);
            current_state = gpio_input_bit_get(key_gpio[key_no].port, key_gpio[key_no].pin);
            if (current_state != key_pre_state[key_no]) {
                if (current_state == RELEASED) {
                    if (key_released_callbacks[key_no]) {
                        key_released_callbacks[key_no](key_no);
                    }
                } else if (current_state == PRESSED) {
                    if (key_pressed_callbacks[key_no]) {
                        key_pressed_callbacks[key_no](key_no);
                    }
                }
                key_pre_state[key_no] = current_state;
            }
        }
    }
}

void int_key_register_callback(KEY_NO key_no, key_callback_t on_pressed, key_callback_t on_released) {
    if (on_pressed) {
        key_pressed_callbacks[key_no] = on_pressed;
    }
    if (on_released) {
        key_released_callbacks[key_no] = on_released;
    }
}

static void init_key_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin) {
    rcu_periph_clock_enable(rcu_gpio);
    gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pin);
}
