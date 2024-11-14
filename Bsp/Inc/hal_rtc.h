//
// Created by 86157 on 2024/11/14.
//

#ifndef HAL_RTC_H
#define HAL_RTC_H

#include "gd32f4xx.h"

void hal_rtc_init(void);
void hal_rtc_clock_read(void);
void hal_rtc_clock_setup(void);
void hal_rtc_alarm_setup(void);

#endif //HAL_RTC_H
