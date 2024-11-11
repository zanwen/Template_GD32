//
// Created by 86157 on 2024/11/11.
//

#include "hal_timer.h"
#include "logger.h"


void hal_timer_basic_init(uint32_t timer_periph, uint8_t prescaler, uint16_t target_frequency) {
    if (timer_periph != TIMER5 && timer_periph != TIMER6) {
        LOG_ERROR("timer_periph must be TIMER5 or TIMER6! ")
        return;
    }
    // 使能定时器时钟
    rcu_periph_clock_enable(timer_periph == TIMER5 ? RCU_TIMER5 : RCU_TIMER6);

    // 复位定时器
    timer_deinit(timer_periph);

    // 配置定时器基本参数
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = prescaler - 1;
    timer_initpara.period = (SystemCoreClock / prescaler / target_frequency - 1);
    timer_init(timer_periph, &timer_initpara);

    // 配置NVIC
    nvic_irq_enable(timer_periph == TIMER5 ? TIMER5_DAC_IRQn : TIMER6_IRQn,
                    HAL_BASIC_TIMER_PRE_PRIORITY,
                    HAL_BASIC_TIMER_SUB_PRIORITY);
    timer_interrupt_enable(timer_periph, TIMER_INT_UP);
    timer_enable(timer_periph);
}

void hal_tim5_dac_irqhandler(void) {
    if (SET == timer_interrupt_flag_get(TIMER5, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
        hal_timer5_overflow_callback();
    }
}

__attribute__((weak)) void hal_timer5_overflow_callback() {

}

void hal_tim6_dac_irqhandler(void) {
    if (SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_FLAG_UP)) {
        timer_interrupt_flag_clear(TIMER6, TIMER_INT_FLAG_UP);
        hal_timer6_overflow_callback();
    }
}

__attribute__((weak)) void hal_timer6_overflow_callback() {

}

