//
// Created by 86157 on 2024/11/25.
//
#include "app.h"
#include "int_led.h"
#include "int_key.h"
#include "int_nixietube.h"
#include "int_buzzer.h"

void app_init(void) {
    int_led_init();
    int_key_init();
    Int_NixieTube_Init();
    int_buzzer_init();
    LOG_DEBUG("app_init done")
}