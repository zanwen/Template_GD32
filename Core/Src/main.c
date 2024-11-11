#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"

void on_read_complete(void);

void gpio_config(void) {
    // 使能GPIOC和GPIOD的时钟
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);

    // 配置PC6为推挽输出
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    // 配置PD15为推挽输出
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_15);
    gpio_af_set(GPIOD, GPIO_AF_2, GPIO_PIN_15);

    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_14);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_14);

    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOD, GPIO_PIN_15);
    gpio_bit_reset(GPIOD, GPIO_PIN_14);
}

void hal_timer5_overflow_callback() {
    gpio_bit_toggle(GPIOD, GPIO_PIN_15);
}

void hal_timer6_overflow_callback() {
    gpio_bit_toggle(GPIOD, GPIO_PIN_14);
}

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    gpio_config();
    hal_timer3_pwm_init(TIMER3, 10, 1000);

    while (1) {
        delay_1ms(1000);
    }
}

void on_read_complete(void) {
    uint8_t cmd = hal_usart0_get_byte();
    LOG_DEBUG("cmd: %#x", cmd)
}

