#include "int_rtc8563.h"
#include "hal_i2c_soft.h"

#define DECIMAL_TO_BCD(val) ((val % 10) | ((val / 10) << 4))
#define BCD_TO_DECIMAL(val) ((val >> 4) * 10 + (val & 0x0F))

char* WEEKDAY_STR[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

static callback_t s_alarm_callabck = NULL;
static callback_t s_timer_callback = NULL;

static void Int_RTC_Clock2BCD(Clock* clk, u8* dest) {
    int year;
    *dest = DECIMAL_TO_BCD(clk->second);
    dest++;
    *dest = DECIMAL_TO_BCD(clk->minute);
    dest++;
    *dest = DECIMAL_TO_BCD(clk->hour);
    dest++;
    *dest = DECIMAL_TO_BCD(clk->day);
    dest++;
    *dest = clk->weekday;
    dest++;
    *dest = DECIMAL_TO_BCD(clk->month);
    if (clk->year / 2100 >= 1) {
        // century flag in month register bit7
        *dest |= 0x80;
    }
    dest++;
    year = clk->year % 100;
    *dest = DECIMAL_TO_BCD(year);
}

static void Int_RTC_Alarm2BCD(Clock* alarm, u8* dest) {
    // 0x80(bit7=1) indicates invalid alarm field
    *dest = alarm->minute >= 0 ? DECIMAL_TO_BCD(alarm->minute) : 0x80;
    dest++;
    *dest = alarm->hour >= 0 ? DECIMAL_TO_BCD(alarm->hour) : 0x80;
    dest++;
    *dest = alarm->day >= 0 ? DECIMAL_TO_BCD(alarm->day) : 0x80;
    dest++;
    *dest = alarm->weekday >= 0 ? alarm->weekday : 0x80;
}

static void Int_RTC_BCD2Clock(Clock* clk, u8* bcd) {
    bit century;
    clk->second = BCD_TO_DECIMAL(*bcd);
    bcd++;
    clk->minute = BCD_TO_DECIMAL(*bcd);
    bcd++;
    clk->hour = BCD_TO_DECIMAL(*bcd);
    bcd++;
    clk->day = BCD_TO_DECIMAL(*bcd);
    bcd++;
    clk->weekday = (*bcd & 0x0F);
    bcd++;
    century = (*bcd >> 7) & 1;
    *bcd &= ~(1 << 7);
    clk->month = BCD_TO_DECIMAL(*bcd);
    bcd++;
    clk->year = BCD_TO_DECIMAL(*bcd);
    clk->year += century ? 2100 : 2000;
}

static void Int_RTC_InterruptCallback() {
    u8 control;
    // static u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);

    if ((control & FLAG_TIE) && (control & FLAG_TF)) {
        if (s_timer_callback) {
            s_timer_callback();
        }
        Int_RTC_ClearTimerFlag();
    }

    if ((control & FLAG_AIE) && (control & FLAG_AF)) {
        if (s_alarm_callabck) {
            s_alarm_callabck();
        }
        Int_RTC_ClearAlarmFlag();
    }
}

void Int_RTC_Init() {
    hal_i2c_soft_init();
    // todo Dri_INT3_RegisterCallabck(Int_RTC_InterruptCallback);
}

void Int_RTC_SetClock(Clock* clk) {
    u8 bcd[CLOCK_REGS_NUM];
    LOG_DEBUG("Int_RTC_SetClock");
    Int_RTC_Clock2BCD(clk, bcd);
    hal_i2c_soft_write(ADDR_DEV, ADDR_CLOCK, bcd, CLOCK_REGS_NUM);
}

void Int_RTC_GetClock(Clock* clk) {
    u8 bcd[CLOCK_REGS_NUM];
    hal_i2c_soft_read(ADDR_DEV, ADDR_CLOCK, bcd, CLOCK_REGS_NUM);
    Int_RTC_BCD2Clock(clk, bcd);
}

void Int_RTC_EnableAlarm() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control |= (1 << CONTROL_BIT_AIE);   // bit1: AIE Alarm Interrupt Enable
    control &= ~(1 << CONTROL_BIT_AF);   // bit3: AF Alarm interrupt Flag
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_DisableAlarm() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control &= ~(1 << CONTROL_BIT_AIE);   // bit1: AIE Alarm Interrupt Enable
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_ClearAlarmFlag() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control &= ~(1 << CONTROL_BIT_AF);   // bit3: AF Alarm interrupt Flag
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_ConfigTimer(Frequence frequence, u8 countdown) {
    u8 config[2];
    // 1. timer control
    config[0] = frequence | FLAG_TE;
    // 2. timer value
    config[1] = countdown;
    hal_i2c_soft_write(ADDR_DEV, ADDR_TIMER_CONTROL, config, 2);
}

void Int_RTC_EnableTimer() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control |= (1 << CONTROL_BIT_TIE);
    control &= ~(1 << CONTROL_BIT_TF);
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_DisableTimer() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control &= ~(1 << CONTROL_BIT_TIE);
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_ClearTimerFlag() {
    u8 control;
    hal_i2c_soft_read(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
    control &= ~(1 << CONTROL_BIT_TF);
    hal_i2c_soft_write(ADDR_DEV, ADDR_INT_CONTROL, &control, 1);
}

void Int_RTC_RegisterAlarmCallback(callback_t callabck) {
    s_alarm_callabck = callabck;
}

void Int_RTC_RegisterTimerCallback(callback_t callabck) {
    s_timer_callback = callabck;
}

void Int_RTC_SetAlarm(Clock* alarm) {
    u8 bcd[ALARM_REGS_NUM];
    Int_RTC_Alarm2BCD(alarm, bcd);
    hal_i2c_soft_write(ADDR_DEV, ADDR_ALARM, bcd, ALARM_REGS_NUM);
}

void Int_RTC_FormatDate(Clock* clk, char* buf) {
    sprintf(buf, "%02d-%02d-%02d", (int)clk->year % 100, (int)clk->month, (int)clk->day);
}

void Int_RTC_FormatTime(Clock* clk, char* buf) {
    sprintf(buf, "%02d:%02d:%02d", (int)clk->hour, (int)clk->minute, (int)clk->second);
}

void Int_RTC_PrintClock(Clock* clk) {
    printf("%02d-%02d-%02d\n", (int)clk->year, (int)clk->month, (int)clk->day);
    printf("weekday: %d\n", (int)clk->weekday);
    printf("%02d:%02d:%02d\n", (int)clk->hour, (int)clk->minute, (int)clk->second);
}