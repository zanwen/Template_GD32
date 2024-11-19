//
// Created by 86157 on 2024/11/10.
//

#ifndef HAL_EXTI_H
#define HAL_EXTI_H
#include "gd32f4xx.h"

#define HAL_EXTI0_GPIO_CLK RCU_GPIOC
#define HAL_EXTI0_GPIO_PORT GPIOC
#define HAL_EXTI0_SOURCE_PORT EXTI_SOURCE_GPIOC
#define HAL_EXTI0_TRIG_TYPE EXTI_TRIG_BOTH

#define HAL_EXTI2_GPIO_CLK RCU_GPIOC
#define HAL_EXTI2_GPIO_PORT GPIOC
#define HAL_EXTI2_SOURCE_PORT EXTI_SOURCE_GPIOC
#define HAL_EXTI2_TRIG_TYPE EXTI_TRIG_FALLING

#define HAL_EXTI5_GPIO_CLK RCU_GPIOB
#define HAL_EXTI5_GPIO_PORT GPIOB
#define HAL_EXTI5_SOURCE_PORT EXTI_SOURCE_GPIOB
#define HAL_EXTI5_TRIG_TYPE EXTI_TRIG_RISING

#define HAL_EXTI_IRQ_PRIORITY 2U
#define HAL_EXTI_IRQ_SUB_PRIORITY 0U

#include <stdbool.h>

typedef enum {
    HAL_EXTI0,
    HAL_EXTI2,
    HAL_EXTI5,
} HAL_EXTI_NO;

void hal_exti0_callback(void);
void hal_exti2_callback(void);
void hal_exti5_callback(void);

void hal_exti_init(HAL_EXTI_NO hal_exti_no, bool only_software_trigger);

#endif//HAL_EXTI_H
