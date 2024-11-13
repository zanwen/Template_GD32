#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"

void on_read_complete(void);

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    // PD12~15
    rcu_periph_clock_enable(RCU_GPIOD);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_af_set(GPIOD, GPIO_AF_2, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    // Tiemr3 PWM CH0~3
    hal_timer_init(RCU_TIMER3, TIMER3, 10, 1000);
    hal_timer_pwm_channel_enable(TIMER3, TIMER_CH_0);
    hal_timer_pwm_channel_enable(TIMER3, TIMER_CH_1);
    hal_timer_pwm_channel_enable(TIMER3, TIMER_CH_2);
    hal_timer_pwm_channel_enable(TIMER3, TIMER_CH_3);

    while (1) {
        delay_1ms(1000);
    }
}

void on_read_complete(void) {
    uint8_t cmd = hal_usart0_get_byte();
    static uint8_t duty_cycle = 0;
    LOG_DEBUG("cmd: %#x", cmd)
    if (cmd == 0x01) {
        duty_cycle += 10;
        if (duty_cycle > 100) {
            duty_cycle = 0;
        }
        hal_timer_pwm_set_duty_cycle(TIMER3, TIMER_CH_0, 10, 1000, duty_cycle);
        hal_timer_pwm_set_duty_cycle(TIMER3, TIMER_CH_1, 10, 1000, duty_cycle);
        hal_timer_pwm_set_duty_cycle(TIMER3, TIMER_CH_2, 10, 1000, duty_cycle);
        hal_timer_pwm_set_duty_cycle(TIMER3, TIMER_CH_3, 10, 1000, duty_cycle);
    }
}

