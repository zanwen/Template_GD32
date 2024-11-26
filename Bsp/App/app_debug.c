//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_key.h"

void app_debug_key_scan(void) {
    int_key_scan();
}

void key_up_callback(KEY_NO key_no) {
    LOG_DEBUG("key up")
}
void key_down_callback(KEY_NO key_no) {
    LOG_DEBUG("key down")
}