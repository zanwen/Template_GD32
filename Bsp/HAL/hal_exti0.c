//
// Created by 86157 on 2024/11/10.
//

#include "hal_exti0.h"
#include "gd32f4xx.h"
#include "logger.h"
#include "systick.h"

// GPIO definitions
#define EXTI0_GPIO_PORT GPIOC
#define EXTI0_GPIO_PIN GPIO_PIN_0
#define EXTI0_GPIO_CLK RCU_GPIOC

// EXTI definitions
#define EXTI0_SOURCE_PORT EXTI_SOURCE_GPIOC
#define EXTI0_SOURCE_PIN EXTI_SOURCE_PIN0
#define EXTI0_LINE EXTI_0
#define EXTI0_IRQn EXTI0_IRQn
#define EXTI0_TRIG_TYPE EXTI_TRIG_BOTH

// NVIC definitions
#define EXTI0_IRQ_PRIORITY 2U
#define EXTI0_IRQ_SUB_PRIORITY 0U

void hal_exti0_init(void) {
    // Enable clocks
    rcu_periph_clock_enable(EXTI0_GPIO_CLK);
    rcu_periph_clock_enable(RCU_SYSCFG);

    // Configure GPIO
    gpio_mode_set(EXTI0_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, EXTI0_GPIO_PIN);

    // Configure EXTI
    syscfg_exti_line_config(EXTI0_SOURCE_PORT, EXTI0_SOURCE_PIN);// interrupt source
    exti_init(EXTI0_LINE, EXTI_INTERRUPT, EXTI0_TRIG_TYPE);       // trigger mode
    exti_interrupt_flag_clear(EXTI0_LINE);                       // pending flag
    exti_interrupt_enable(EXTI0_LINE);                           // enable exti interrupt

    // Configure NVIC
    nvic_irq_enable(EXTI0_IRQn, EXTI0_IRQ_PRIORITY, EXTI0_IRQ_SUB_PRIORITY);
}

void hal_exti0_handle_irq(void) {
    static uint64_t start, elapsed;
    if (gpio_input_bit_get(EXTI0_GPIO_PORT, EXTI0_GPIO_PIN) == RESET) {
        start = get_systick_us();
    } else {
        // rising edge - falling edge
        elapsed = (get_systick_us() - start) / 1000;
        LOG_INFO("elapsed: %llu ms", elapsed);
        if (elapsed > 200 && elapsed < 2000) {
            LOG_DEBUG("short press");
        } else if (elapsed > 2000) {
            LOG_DEBUG("short press");
            LOG_DEBUG("long press");
        }
    }
}