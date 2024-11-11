//
// Created by 86157 on 2024/11/10.
//

#include "hal_exti.h"
#include "systick.h"

static void hal_exti_init_hardware(rcu_periph_enum gpio_clk,
                                   uint32_t gpio_port, uint32_t gpio_pin,
                                   exti_line_enum exti_line,
                                   uint8_t source_port, uint8_t source_pin,
                                   exti_trig_type_enum trig_type,
                                   uint8_t nvic_irq,
                                   uint8_t irq_priority, uint8_t irq_sub_priority) {
    // Enable clocks
    rcu_periph_clock_enable(gpio_clk);
    rcu_periph_clock_enable(RCU_SYSCFG);

    // Configure GPIO
    gpio_mode_set(gpio_port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, gpio_pin);

    // Configure EXTI
    syscfg_exti_line_config(source_port, source_pin);// interrupt source
    exti_init(exti_line, EXTI_INTERRUPT, trig_type); // trigger mode
    exti_interrupt_flag_clear(exti_line);            // pending flag
    exti_interrupt_enable(exti_line);                // enable exti interrupt

    // Configure NVIC
    nvic_irq_enable(nvic_irq, irq_priority, irq_sub_priority);
}

static void hal_exti_init_software(exti_line_enum exti_line,
                                   exti_trig_type_enum trig_type,
                                   uint8_t nvic_irq,
                                   uint8_t irq_priority, uint8_t irq_sub_priority) {
    exti_init(exti_line, EXTI_INTERRUPT, trig_type);// trigger mode
    exti_interrupt_flag_clear(exti_line);           // pending flag
    exti_interrupt_enable(exti_line);               // enable exti interrupt
    nvic_irq_enable(nvic_irq, irq_priority, irq_sub_priority);
}

void hal_exti0_handle_irq(void) {
    if (RESET != exti_interrupt_flag_get(EXTI_0)) {
        hal_exti0_callback();
        exti_interrupt_flag_clear(EXTI_0);
    }
}

__attribute__((weak)) void hal_exti0_callback(void) {
}

void hal_exti2_handle_irq(void) {
    if (RESET != exti_interrupt_flag_get(EXTI_2)) {
        hal_exti2_callback();
        exti_interrupt_flag_clear(EXTI_2);
    }
}

__attribute__((weak)) void hal_exti2_callback(void) {
}

void hal_exti_init(HAL_EXTI_NO hal_exti_no, bool only_software_trigger) {
    rcu_periph_enum gpio_clk;
    uint32_t gpio_port;
    uint32_t gpio_pin;
    exti_line_enum exti_line;
    uint8_t source_port;
    uint8_t source_pin;
    uint8_t nvic_irq;
    exti_trig_type_enum trig_type;
    switch (hal_exti_no) {
        case HAL_EXTI0:
            gpio_clk = HAL_EXTI0_GPIO_CLK;
            gpio_port = HAL_EXTI0_GPIO_PORT;
            gpio_pin = GPIO_PIN_0;
            exti_line = EXTI_0;
            source_port = HAL_EXTI0_SOURCE_PORT;
            source_pin = EXTI_SOURCE_PIN0;
            nvic_irq = EXTI0_IRQn;
            trig_type = HAL_EXTI0_TRIG_TYPE;
            break;
        case HAL_EXTI2:
            gpio_clk = HAL_EXTI2_GPIO_CLK;
            gpio_port = HAL_EXTI2_GPIO_PORT;
            gpio_pin = GPIO_PIN_2;
            exti_line = EXTI_2;
            source_port = HAL_EXTI2_SOURCE_PORT;
            source_pin = EXTI_SOURCE_PIN2;
            nvic_irq = EXTI2_IRQn;
            trig_type = HAL_EXTI2_TRIG_TYPE;
            break;
        default:
            return;
    }
    if (only_software_trigger) {
        hal_exti_init_software(exti_line, trig_type, nvic_irq,
                               HAL_EXTI_IRQ_PRIORITY, HAL_EXTI_IRQ_SUB_PRIORITY);
    } else {
        hal_exti_init_hardware(gpio_clk, gpio_port, gpio_pin,
                               exti_line, source_port, source_pin,trig_type,
                               nvic_irq,
                               HAL_EXTI_IRQ_PRIORITY, HAL_EXTI_IRQ_SUB_PRIORITY);
    }

}
