//
// Created by 86157 on 2024/11/10.
//

#include "hal_exti0.h"
#include "gd32f4xx.h"

#define LED_GPIO_PORT           GPIOB
#define LED_GPIO_PIN            GPIO_PIN_2
#define LED_GPIO_CLK            RCU_GPIOB

// GPIO definitions
#define EXTI0_GPIO_PORT         GPIOA
#define EXTI0_GPIO_PIN          GPIO_PIN_0
#define EXTI0_GPIO_CLK          RCU_GPIOA

// EXTI definitions
#define EXTI0_SOURCE_PORT       EXTI_SOURCE_GPIOA
#define EXTI0_SOURCE_PIN        EXTI_SOURCE_PIN0
#define EXTI0_LINE              EXTI_0
#define EXTI0_IRQn              EXTI0_IRQn

// NVIC definitions
#define EXTI0_IRQ_PRIORITY      2U
#define EXTI0_IRQ_SUB_PRIORITY  0U

void hal_exti0_init(void) {
    // Enable clocks
    rcu_periph_clock_enable(EXTI0_GPIO_CLK);
    rcu_periph_clock_enable(LED_GPIO_CLK);
    rcu_periph_clock_enable(RCU_SYSCFG);

    // Configure GPIO
    gpio_mode_set(EXTI0_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, EXTI0_GPIO_PIN);
    gpio_mode_set(LED_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN);
    gpio_output_options_set(LED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN);

    // Configure EXTI
    syscfg_exti_line_config(EXTI0_SOURCE_PORT, EXTI0_SOURCE_PIN);  // interrupt source
    exti_init(EXTI0_LINE, EXTI_INTERRUPT, EXTI_TRIG_BOTH);         // trigger mode
    exti_interrupt_flag_clear(EXTI0_LINE);                         // pending flag
    exti_interrupt_enable(EXTI0_LINE);                             // enable exti interrupt

    // Configure NVIC
    nvic_irq_enable(EXTI0_IRQn, EXTI0_IRQ_PRIORITY, EXTI0_IRQ_SUB_PRIORITY);
}

void hal_exti0_handle_irq(void) {
    if (gpio_input_bit_get(EXTI0_GPIO_PORT, EXTI0_GPIO_PIN) == SET) {
        gpio_bit_write(LED_GPIO_PORT, LED_GPIO_PIN, SET);
    } else if (gpio_input_bit_get(EXTI0_GPIO_PORT, EXTI0_GPIO_PIN) == RESET) {
        gpio_bit_write(LED_GPIO_PORT, LED_GPIO_PIN, RESET);
    }
}