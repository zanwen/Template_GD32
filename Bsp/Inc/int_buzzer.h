//
// Created by 86157 on 2024/11/11.
//

#ifndef INT_BUZZER_H
#define INT_BUZZER_H

#define INT_BUZZER_GPIO_RCU RCU_GPIOB
#define INT_BUZZER_GPIO_PORT GPIOB
#define INT_BUZZER_GPIO_PIN GPIO_PIN_9


#define INT_BUZZER_TIMER_RCU RCU_TIMER1
#define INT_BUZZER_TIMER TIMER1
#define INT_BUZZER_TIMER_CH TIMER_CH_1

#define INT_BUZZER_PWM_INIT_FREQ 1000

void int_buzzer_init(void);

#include "gd32f4xx.h"

void int_buzzer_buzz(uint16_t period, uint16_t duration);

void int_buzzer_stop(void);

void int_buzzer_playdemo();
void int_buzzer_playdemo_task();

#endif //INT_BUZZER_H
