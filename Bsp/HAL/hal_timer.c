//
// Created by 86157 on 2024/11/11.
//

#include "hal_timer.h"
#include "logger.h"

static uint32_t sg_pwm_timer_period = 0;


void hal_timer_basic_init(uint32_t timer_periph, uint16_t prescaler, uint16_t target_frequency) {
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

void hal_timer_pwm_init(rcu_periph_enum periph_clk, uint32_t timer_periph, uint16_t channel, uint16_t prescaler, uint16_t target_freq) {
    rcu_periph_clock_enable(periph_clk);

    timer_deinit(timer_periph);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);

    sg_pwm_timer_period = SystemCoreClock / prescaler / target_freq - 1;
    timer_parameter_struct timer_initpara;
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler = prescaler - 1;
    timer_initpara.period = sg_pwm_timer_period;
    timer_init(timer_periph, &timer_initpara);

    // channel output
    timer_channel_output_state_config(timer_periph, channel, TIMER_CCX_ENABLE);
    timer_channel_output_pulse_value_config(timer_periph, channel, 0);
    timer_channel_output_mode_config(timer_periph, channel, TIMER_OC_MODE_PWM0);

    timer_enable(timer_periph);
    LOG_DEBUG("timer enabled. period = %lu", sg_pwm_timer_period);
}

void hal_timer_pwm_set_duty_cycle(uint32_t timer_periph, uint16_t channel, uint16_t duty_cycle) {
    LOG_DEBUG("duty_cycle = %d", duty_cycle)
    uint16_t compare_value = (sg_pwm_timer_period * duty_cycle) / 100;
    timer_channel_output_pulse_value_config(timer_periph, channel, compare_value);
}

void hal_timer_disable(uint32_t timer_periph) {
    timer_disable(timer_periph);
}

