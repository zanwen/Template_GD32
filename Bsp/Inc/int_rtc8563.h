#ifndef __INT_RTC_H__
#define __INT_RTC_H__

#include "gd32f4xx.h"
#include "logger.h"
#include "com_typedef.h"

#define ADDR_DEV           0xa2
#define ADDR_CLOCK         0x02
#define ADDR_ALARM         0x09
#define ADDR_INT_CONTROL   0x01
#define ADDR_TIMER_CONTROL 0x0E

#define CLOCK_REGS_NUM 7
#define ALARM_REGS_NUM 4

// timer enable
#define FLAG_TE  0x80
// timer interrupt enable
#define FLAG_TIE (1 << CONTROL_BIT_TIE)
// alarm interrupt enable
#define FLAG_AIE (1 << CONTROL_BIT_AIE)
// timer flag(interruptted)
#define FLAG_TF  (1 << CONTROL_BIT_TF)
// alarm flag
#define FLAG_AF  (1 << CONTROL_BIT_AF)

// Timer Interrupt Enable
#define CONTROL_BIT_TIE 0
// Alarm Interrupt Enable
#define CONTROL_BIT_AIE 1
// Timer interrupt Flag
#define CONTROL_BIT_TF  2
// Alarm interrupt Flag
#define CONTROL_BIT_AF  3

// timer frequence selection
typedef enum {
    HZ_4096 = 0,
    HZ_64 = 1,
    HZ_1 = 2,
    // 1/60HZ
    HZ_1_60 = 3
} Frequence;

typedef struct {
    int year;
    char month, day, weekday, hour, minute, second;
} Clock;

extern char* WEEKDAY_STR[7];

void Int_RTC_Init();

void Int_RTC_SetClock(Clock* clk);
void Int_RTC_GetClock(Clock* clk);

void Int_RTC_SetAlarm(Clock* alarm);
void Int_RTC_EnableAlarm();
void Int_RTC_DisableAlarm();
void Int_RTC_ClearAlarmFlag();

void Int_RTC_ConfigTimer(Frequence frequence, u8 countdown);
void Int_RTC_EnableTimer();
void Int_RTC_DisableTimer();
void Int_RTC_ClearTimerFlag();

void Int_RTC_RegisterAlarmCallback(callback_t callabck);
void Int_RTC_RegisterTimerCallback(callback_t callabck);

void Int_RTC_FormatDate(Clock* clk, char* buf);
void Int_RTC_FormatTime(Clock* clk, char* buf);
void Int_RTC_PrintClock(Clock* clk);

#endif /* __INT_RTC_H__ */