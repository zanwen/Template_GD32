//
// Created by 86157 on 2024/11/26.
//

#ifndef __MID_TASK_DEF_H
#define __MID_TASK_DEF_H

#include "gd32f4xx.h"

typedef void (*mid_task_callbale)(void);

typedef struct {
    uint16_t task_id;
    uint16_t last_run_tick;
    uint16_t yield_tick;
    mid_task_callbale callable;
} mid_task;

#endif//__MID_TASK_DEF_H
