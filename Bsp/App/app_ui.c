//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_led.h"

void app_ui_led_flow(void) {
//    LOG_DEBUG("app_ui_led_flow invoke")
    static uint8_t led_index = LED1;
    int_led_on(led_index, 0);
    led_index++;
    if (led_index == LED_SIZE) {
        int_led_off_all(0);
        led_index = LED1;
    }
}