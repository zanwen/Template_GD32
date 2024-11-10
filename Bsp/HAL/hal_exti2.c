//
// Created by 86157 on 2024/11/10.
//

#include "hal_exti2.h"
#include "gd32f4xx.h"
#include "logger.h"
#include "systick.h"

// GPIO definitions
#define EXTI2_GPIO_PORT GPIOC
#define EXTI2_GPIO_PIN GPIO_PIN_2
#define EXTI2_GPIO_CLK RCU_GPIOC

// EXTI definitions
#define EXTI2_SOURCE_PORT EXTI_SOURCE_GPIOC
#define EXTI2_SOURCE_PIN EXTI_SOURCE_PIN2
#define EXTI2_LINE EXTI_2
#define EXTI2_IRQn EXTI2_IRQn
#define EXTI2_TRIG_TYPE EXTI_TRIG_FALLING

// NVIC definitions
#define EXTI2_IRQ_PRIORITY 2U
#define EXTI2_IRQ_SUB_PRIORITY 0U

void hal_exti2_init(void) {
    // Enable clocks
    rcu_periph_clock_enable(EXTI2_GPIO_CLK);
    rcu_periph_clock_enable(RCU_SYSCFG);

    // Configure GPIO
    gpio_mode_set(EXTI2_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, EXTI2_GPIO_PIN);

    // Configure EXTI
    syscfg_exti_line_config(EXTI2_SOURCE_PORT, EXTI2_SOURCE_PIN);// interrupt source
    exti_init(EXTI2_LINE, EXTI_INTERRUPT, EXTI2_TRIG_TYPE);       // trigger mode
    exti_interrupt_flag_clear(EXTI2_LINE);                       // pending flag
    exti_interrupt_enable(EXTI2_LINE);                           // enable exti interrupt

    // Configure NVIC
    nvic_irq_enable(EXTI2_IRQn, EXTI2_IRQ_PRIORITY, EXTI2_IRQ_SUB_PRIORITY);
}

uint64_t start_tick = 0;
void hal_exti2_handle_irq(void) {
    LOG_DEBUG("falling edge triggering");
}