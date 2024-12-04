//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_key.h"
#include "scheduler.h"
#include "int_led.h"
#include "int_nixietube.h"
#include "int_buzzer.h"

extern task_t tasks[];

void app_debug_key_scan(void) {
    int_key_scan();
}

void key_up_callback(KEY_NO key_no) {
    LOG_DEBUG("key up")
}

void reset() {
    int_led_off_all(0);
    Int_NixieTube_Display(8, 0);
    int_buzzer_stop();
}

void key_down_callback(KEY_NO key_no) {
    LOG_DEBUG("key down")
    reset();
    for(uint8_t i = 1; i <= 4; ++i) {
        tasks[i].task_state = (key_no + 1) == i ? TASK_READY : TASK_DISABLED;
    }
}