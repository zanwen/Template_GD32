//
// Created by 86157 on 2024/11/11.
//

#include "int_buzzer.h"
#include "hal_timer.h"
#include "systick.h"

// 两只老虎
//					 	C`	   D`     E`   F`	  G`	A`	  B`    C``
//                   	1     2      3    4      5     6    7     8   不是下标，是音频
static uint16_t hz[] = {1047, 1175, 1319, 1397, 1568, 1760, 1976, 2093};
//                   	0     1      2    3      4     5    6    7    数组下标

// 简谱
static uint8_t notes[] = {
        1, 2, 3, 1, 1, 2, 3, 1, 3, 4, 5, 3, 4, 5,
        5, 6, 5, 4, 3, 1, 5, 6, 5, 4, 3, 1, 1, 5, 1, 1, 5, 1
};

// 音长
static uint8_t durations[] = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 8,
        3, 1, 3, 1, 4, 4, 3, 1, 3, 1, 4, 4, 4, 4, 8, 4, 4, 8
};

static void gpio_init() {
    rcu_periph_clock_enable(INT_BUZZER_GPIO_RCU);

    gpio_mode_set(INT_BUZZER_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, INT_BUZZER_GPIO_PIN);
    gpio_output_options_set(INT_BUZZER_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, INT_BUZZER_GPIO_PIN);
    gpio_af_set(INT_BUZZER_GPIO_PORT, GPIO_AF_1, INT_BUZZER_GPIO_PIN);

    gpio_bit_reset(INT_BUZZER_GPIO_PORT, INT_BUZZER_GPIO_PIN);
}

void int_buzzer_init(void) {
    gpio_init();
    hal_timer_pwm_init(INT_BUZZER_TIMER_RCU, INT_BUZZER_TIMER,
                       INT_BUZZER_TIMER_CH, INT_BUZZER_PWM_PRESCALER,
                       INT_BUZZER_PWM_INIT_PERIOD);
}

void int_buzzer_buzz(uint16_t freq, uint16_t duration) {
    uint16_t period = SystemCoreClock / (INT_BUZZER_PWM_PRESCALER * freq) - 1;
    hal_timer_pwm_set_period(
            INT_BUZZER_TIMER, INT_BUZZER_PWM_PRESCALER, period);
    hal_timer_pwm_set_duty_cycle(
            INT_BUZZER_TIMER, INT_BUZZER_TIMER_CH, period, 50);
    hal_timer_enable(INT_BUZZER_TIMER);
    if (duration) {
        delay_1ms(duration);
    }
}

void int_buzzer_stop(void) {
    hal_timer_disable(INT_BUZZER_TIMER);
}


void int_buzzer_playdemo() {
    uint16_t i;
    uint16_t len = sizeof(notes) / sizeof(notes[0]);

    for (i = 0; i < len; i++) {
        int_buzzer_buzz(hz[notes[i]], durations[i] * 50);
        int_buzzer_stop(); // 每播放一个音，适当停顿一下，建议
        delay_1ms(5);

    }
    int_buzzer_stop();
}