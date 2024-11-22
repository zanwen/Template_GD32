//
// Created by 86157 on 2024/11/22.
//

#ifndef HAL_PMU_H
#define HAL_PMU_H

#include "gd32f4xx.h"

void hal_pmu_init(void);
void hal_pmu_sleep(void);
void hal_pmu_deepsleep(void);
void hal_pmu_standby(void);

#endif//HAL_PMU_H
