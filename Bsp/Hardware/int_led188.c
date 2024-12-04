//
// Created by 86157 on 2024/12/4.
//

#include "int_led188.h"
#include "logger.h"

#define PIN1_OUT(val)            \
    gpio_out(GPIOE, GPIO_PIN_7); \
    gpio_bit_write(GPIOE, GPIO_PIN_7, (val))

#define PIN2_OUT(val)            \
    gpio_out(GPIOE, GPIO_PIN_9); \
    gpio_bit_write(GPIOE, GPIO_PIN_9, (val))

#define PIN3_OUT(val)            \
    gpio_out(GPIOE, GPIO_PIN_11); \
    gpio_bit_write(GPIOE, GPIO_PIN_11, (val))

#define PIN4_OUT(val)            \
    gpio_out(GPIOE, GPIO_PIN_13); \
    gpio_bit_write(GPIOE, GPIO_PIN_13, (val))

#define PIN5_OUT(val)            \
    gpio_out(GPIOE, GPIO_PIN_15); \
    gpio_bit_write(GPIOE, GPIO_PIN_15, (val))

#define GPIO_ALL_IN()           \
    gpio_in(GPIOE, GPIO_PIN_7); \
    gpio_in(GPIOE, GPIO_PIN_9); \
    gpio_in(GPIOE, GPIO_PIN_11); \
    gpio_in(GPIOE, GPIO_PIN_13); \
    gpio_in(GPIOE, GPIO_PIN_15)

#define A3 (1 << 0)
#define B3 (1 << 1)
#define C3 (1 << 2)
#define D3 (1 << 3)
#define E3 (1 << 4)
#define F3 (1 << 5)
#define G3 (1 << 6)

#define A2 (1 << 7)
#define B2 (1 << 8)
#define C2 (1 << 9)
#define D2 (1 << 10)
#define E2 (1 << 11)
#define F2 (1 << 12)
#define G2 (1 << 13)

#define B1 (1 << 14)
#define C1 (1 << 15)
#define L1 (1 << 16)
#define L2 (1 << 17)

volatile uint32_t led_state = 0;

static inline void gpio_in(uint32_t gpio_periph, uint32_t pin) {
    gpio_mode_set(gpio_periph, GPIO_MODE_INPUT, GPIO_PUPD_NONE, pin);
}

static inline void gpio_out(uint32_t gpio_periph, uint32_t pin) {
    gpio_mode_set(gpio_periph, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(gpio_periph, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}

void int_led188_init(void) {
    rcu_periph_clock_enable(RCU_GPIOE);
    GPIO_ALL_IN();
}

uint8_t num_codes[10] = {
        0x3f,
        0x06,
        0x5b,
        0x4f,
        0x66,
        0x6d,
        0x7d,
        0x07,
        0x7f,
        0x6f,
};

void int_led188_refresh(void) {
    static uint8_t stage = 0;
    switch (stage) {
        case 0:
            GPIO_ALL_IN();
            PIN1_OUT(1);
            if (led_state & A3) {
                PIN2_OUT(0);
            }
            if (led_state & C3) {
                PIN3_OUT(0);
            }
            if (led_state & E3) {
                PIN4_OUT(0);
            }
            break;
        case 1:
            GPIO_ALL_IN();
            PIN2_OUT(1);
            if (led_state & B3) {
                PIN1_OUT(0);
            }
            if (led_state & A2) {
                PIN3_OUT(0);
            }
            if (led_state & C1) {
                PIN4_OUT(0);
            }
            if (led_state & L2) {
                PIN5_OUT(0);
            }
            break;
        case 2:
            GPIO_ALL_IN();
            PIN3_OUT(1);
            if (led_state & D3) {
                PIN1_OUT(0);
            }
            if (led_state & B2) {
                PIN2_OUT(0);
            }
            if (led_state & B1) {
                PIN4_OUT(0);
            }
            if (led_state & L1) {
                PIN5_OUT(0);
            }
            break;
        case 3:
            GPIO_ALL_IN();
            PIN4_OUT(1);
            if (led_state & F3) {
                PIN1_OUT(0);
            }
            if (led_state & D2) {
                PIN2_OUT(0);
            }
            if (led_state & C2) {
                PIN3_OUT(0);
            }
            break;
        case 4:
            GPIO_ALL_IN();
            PIN5_OUT(1);
            if (led_state & G3) {
                PIN1_OUT(0);
            }
            if (led_state & E2) {
                PIN2_OUT(0);
            }
            if (led_state & F2) {
                PIN3_OUT(0);
            }
            if (led_state & G2) {
                PIN4_OUT(0);
            }
            break;
    }
    stage = (stage + 1) % 5;
}

void int_led188_set_num(void) {
    static uint8_t num = 0;
    led_state = 0;
    led_state |= num_codes[num % 10];
    if (num >= 10 && num < 100) {
        led_state |= num_codes[num / 10] << 7;
    } else if (num == 100){
        led_state |= num_codes[0] << 7;
        led_state |= num_codes[1] << 14;
    }
    led_state |= 3 << 16;
    num = (num + 1) % 101;
}
