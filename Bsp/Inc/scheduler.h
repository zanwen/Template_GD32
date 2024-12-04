//
// Created by 86157 on 2024/11/25.
//

#ifndef __SCHEDULER_H
#define __SCHEDULER_H

#include "gd32f4xx.h"
#include "stdbool.h"

typedef void (*task_function_t)(void);

typedef enum {
    TASK_READY,
    TASK_WAITING,
    TASK_DISABLED
} task_state_t;

typedef struct {
    task_state_t  task_state;
    uint16_t task_interval_ms;
    uint16_t task_time_waiting_ms;
    task_function_t task_function;
} task_t;

void task_update(void);
void task_execute(void);

#endif//__SCHEDULER_H
