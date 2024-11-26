//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_led.h"
#include "int_key.h"

void app_init(void) {
    int_led_init();
    int_key_init();
    LOG_DEBUG("app_init done")
}