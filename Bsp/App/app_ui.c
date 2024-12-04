//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_led.h"
#include "int_nixietube.h"

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

void app_ui_led_blink_beat(void) {
    static uint16_t beat_count = 0;
    int_led_toggle(LED3, 0);
    if (beat_count % 2 == 0) {
        int_led_toggle(LED2, 0);
    }
    if (beat_count % 4 == 0) {
        int_led_toggle(LED1, 0);
    }
    beat_count++;
    if (beat_count >= 8) {
        beat_count = 1;
    }
}

void app_ui_nixietube(void) {
    static uint8_t chip = 0;
    Int_NixieTube_Display(chip, chip);
    chip++;
    if (chip >= 8) {
        chip = 0;
    }
}