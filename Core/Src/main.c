#include "hal_exti.h"
#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

void on_read_complete(void);

void gpio_config(void) {
    // 使能GPIOC和GPIOD的时钟
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_GPIOD);

    // 配置PC6为推挽输出
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);

    // 配置PD15为推挽输出
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

    gpio_bit_reset(GPIOC, GPIO_PIN_6);
    gpio_bit_reset(GPIOD, GPIO_PIN_15);
}

void timer5_config(void) {
    // 使能TIMER5时钟
    rcu_periph_clock_enable(RCU_TIMER5);

    // 复位TIMER5
    timer_deinit(TIMER5);

    // 配置TIMER5基本参数
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.period = (SystemCoreClock / 10000 - 1);
    timer_init(TIMER5, &timer_initpara);

    nvic_irq_enable(TIMER5_DAC_IRQn, 2, 0);
    timer_interrupt_enable(TIMER5, TIMER_INT_UP);
    timer_enable(TIMER5);
}

void TIM6_DAC_IRQHandler() { // for startup_stm32.s
    if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        //LOG_DEBUG("before gpio_bit_toggle")
        // 引脚高低切换
        gpio_bit_toggle(GPIOD, GPIO_PIN_15);
    }
}

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    gpio_config();
    LOG_DEBUG("before timer5_config")
    timer5_config();
    LOG_DEBUG("after timer5_config")

    while (1) {
        delay_1ms(1000);
    }
}

void on_read_complete(void) {
    uint8_t cmd = hal_usart0_get_byte();
    LOG_DEBUG("cmd: %#x", cmd);
}

