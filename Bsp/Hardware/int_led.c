#include "../Inc/int_led.h"
#include "../Inc/com_util.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"
#include "systick.h"

#define LED_SW_RCU_GPIO RCU_GPIOC
#define LED_SW_PORT GPIOC
#define LED_SW_PIN GPIO_PIN_6

#define LED1_RCU_GPIO RCU_GPIOD
#define LED1_PORT GPIOD
#define LED1_PIN GPIO_PIN_8

#define LED2_RCU_GPIO RCU_GPIOD
#define LED2_PORT GPIOD
#define LED2_PIN GPIO_PIN_9

#define LED3_RCU_GPIO RCU_GPIOD
#define LED3_PORT GPIOD
#define LED3_PIN GPIO_PIN_10

#define LED4_RCU_GPIO RCU_GPIOD
#define LED4_PORT GPIOD
#define LED4_PIN GPIO_PIN_11

#define LED5_RCU_GPIO RCU_GPIOD
#define LED5_PORT GPIOD
#define LED5_PIN GPIO_PIN_12

#define LED6_RCU_GPIO RCU_GPIOD
#define LED6_PORT GPIOD
#define LED6_PIN GPIO_PIN_13

#define LED7_RCU_GPIO RCU_GPIOD
#define LED7_PORT GPIOD
#define LED7_PIN GPIO_PIN_14

#define LED8_RCU_GPIO RCU_GPIOD
#define LED8_PORT GPIOD
#define LED8_PIN GPIO_PIN_15

static gpio_port_pin_t led_gpio[LED_SIZE] = {
        {LED1_PORT, LED1_PIN, LED1_RCU_GPIO},
        {LED2_PORT, LED2_PIN, LED2_RCU_GPIO},
        {LED3_PORT, LED3_PIN, LED3_RCU_GPIO},
        {LED4_PORT, LED4_PIN, LED4_RCU_GPIO},
        {LED5_PORT, LED5_PIN, LED5_RCU_GPIO},
        {LED6_PORT, LED6_PIN, LED6_RCU_GPIO},
        {LED7_PORT, LED7_PIN, LED7_RCU_GPIO},
        {LED8_PORT, LED8_PIN, LED8_RCU_GPIO}};

static void init_led_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin);

void int_led_init(void) {
    init_led_gpio(LED_SW_RCU_GPIO, LED_SW_PORT, LED_SW_PIN);
    for (int i = 0; i < LED_SIZE; i++) {
        init_led_gpio(led_gpio[i].rcu_gpio, led_gpio[i].port, led_gpio[i].pin);
    }

    gpio_bit_reset(LED_SW_PORT, LED_SW_PIN);
    int_led_off_all(0);
}
void int_led_trotting_horse_lamp(LED_INDEX start, LED_INDEX end,
                                 int32_t step, uint32_t delay_ms) {
    if (start <= end) {
        for (int32_t led_index = start; led_index <= end; led_index += step) {
            int_led_on(led_index, delay_ms);
            int_led_off(led_index, 0);
        }
    } else {
        for (int32_t led_index = start; led_index >= end; led_index -= step) {
            int_led_on(led_index, delay_ms);
            int_led_off(led_index, 0);
        }
    }
}

void int_led_waterfall_lamp(LED_INDEX start, LED_INDEX end, int32_t step, uint32_t delay_ms) {
    if (start <= end) {
        for (int32_t led_index = start; led_index <= end; led_index += step) {
            int_led_on(led_index, delay_ms);
        }
    } else {
        for (int32_t led_index = start; led_index >= end; led_index -= step) {
            int_led_on(led_index, delay_ms);
        }
    }
}

void int_led_on(LED_INDEX led_index, uint32_t delay_ms) {
    gpio_bit_write(led_gpio[led_index].port, led_gpio[led_index].pin, LED_ON);
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}

void int_led_off(LED_INDEX led_index, uint32_t delay_ms) {
    gpio_bit_write(led_gpio[led_index].port, led_gpio[led_index].pin, LED_OFF);
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}

void int_led_on_all(uint32_t delay_ms) {
    for (uint32_t i = 0; i < LED_SIZE; i++) {
        gpio_bit_write(led_gpio[i].port, led_gpio[i].pin, LED_ON);
    }
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}
void int_led_off_all(uint32_t delay_ms) {
    for (uint32_t i = 0; i < LED_SIZE; i++) {
        gpio_bit_write(led_gpio[i].port, led_gpio[i].pin, LED_OFF);
    }
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}
void int_led_control(LED_INDEX leds[], uint8_t len, bit_status state, uint32_t delay_ms) {
    for (uint8_t i = 0; i < len; i++) {
        if (leds[i] < LED_SIZE) {
            if (state == LED_ON) {
                int_led_on(leds[i], 0);
            } else {
                int_led_off(leds[i], 0);
            }
        }
    }
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}
void int_led_turn(LED_INDEX led_index, bit_status led_status, uint32_t delay_ms) {
    gpio_bit_write(led_gpio[led_index].port, led_gpio[led_index].pin, led_status);
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}
void int_led_toggle(LED_INDEX led_index, uint32_t delay_ms) {
    gpio_bit_toggle(led_gpio[led_index].port, led_gpio[led_index].pin);
    if (delay_ms) {
        delay_1ms(delay_ms);
    }
}

static void init_led_gpio(rcu_periph_enum rcu_gpio, uint32_t port, uint32_t pin) {
    rcu_periph_clock_enable(rcu_gpio);
    gpio_mode_set(port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, pin);
    gpio_output_options_set(port, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, pin);
}
