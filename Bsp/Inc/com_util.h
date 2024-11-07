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

typedef void (*callback_t)(void);

#endif//COM_UTIL_H
