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

void key_up_callback(KEY_NO key_no);
void key_down_callback(KEY_NO key_no);

static void init_key_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin);

static gpio_port_pin_t key_gpio[KEY_SIZE] = {
        {KEY1_GPIO_PORT, KEY1_GPIO_PIN, RCU_GPIOC},
        {KEY2_GPIO_PORT, KEY2_GPIO_PIN, RCU_GPIOC},
        {KEY3_GPIO_PORT, KEY3_GPIO_PIN, RCU_GPIOC},
        {KEY4_GPIO_PORT, KEY4_GPIO_PIN, RCU_GPIOC}};


static FlagStatus key_pre_state[KEY_SIZE];

void int_key_init(void) {
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        init_key_gpio(key_gpio[key_no].rcu_gpio, key_gpio[key_no].port, key_gpio[key_no].pin);
    }
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        key_pre_state[key_no] = RELEASED;
    }
}
void int_key_scan(void) {
    for (KEY_NO key_no = KEY1; key_no < KEY_SIZE; ++key_no) {
        FlagStatus current_state = gpio_input_bit_get(key_gpio[key_no].port, key_gpio[key_no].pin);
        if (current_state != key_pre_state[key_no]) {
            if (current_state == RELEASED) {
                key_up_callback(key_no);
            } else if (current_state == PRESSED) {
                key_down_callback(key_no);
            }
            key_pre_state[key_no] = current_state;
        }
    }
}

__attribute__((weak)) void key_up_callback(KEY_NO key_no) {

}

__attribute__((weak)) void key_down_callback(KEY_NO key_no) {

}

static void init_key_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin) {
    rcu_periph_clock_enable(rcu_gpio);
    gpio_mode_set(port, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, pin);
}
