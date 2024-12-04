//
// Created by 86157 on 2024/11/25.
//

#include "scheduler.h"
#include "app.h"
#include "int_led188.h"

task_t tasks[] = {
        TASK_READY,
        1,
        0,
        int_led188_refresh,
        TASK_READY,
        1000,
        0,
        int_led188_set_num,
};

static const uint8_t TASK_SIZE = sizeof(tasks) / sizeof(tasks[0]);

void task_update(void) {
    for (uint16_t i = 0; i < TASK_SIZE; ++i) {
        if (tasks[i].task_state == TASK_DISABLED) {
            continue;
        }
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
            tasks[i].task_function();
            tasks[i].task_state = TASK_WAITING;
        }
    }
}
