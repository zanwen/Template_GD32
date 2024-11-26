//
// Created by 86157 on 2024/11/25.
//

#include "scheduler.h"
#include "app.h"

static task_t tasks[] = {
        TASK_READY, 400, 0, app_ui_led_flow,
        TASK_READY, 20, 0, app_debug_key_scan};

static const uint8_t TASK_SIZE = sizeof(tasks) / sizeof(tasks[0]);

void task_update(void) {
    for (uint16_t i = 0; i < TASK_SIZE; ++i) {
        tasks[i].task_time_waiting_ms++;
        if (tasks[i].task_time_waiting_ms >= tasks[i].task_interval_ms) {
            tasks[i].task_state = TASK_READY;
            tasks[i].task_time_waiting_ms = 0;
        }
    }
}

void task_execute(void) {
    for (uint16_t i = 0; i < TASK_SIZE; ++i) {
        if (tasks[i].task_state == TASK_READY) {
            tasks[i].task_state = TASK_RUNNING;
            tasks[i].task_function();
            tasks[i].task_state = TASK_WAITING;
        }
    }
}
