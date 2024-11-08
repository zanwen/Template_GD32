//
// Created by 86157 on 2024/11/5.
//

#ifndef COM_UTIL_H
#define COM_UTIL_H

#include "gd32f4xx_rcu.h"

typedef struct {
    uint32_t port;
    uint32_t pin;
    rcu_periph_enum rcu_gpio;
} gpio_port_pin_t;

uint32_t com_util_min(uint32_t a, uint32_t b);

#endif//COM_UTIL_H
