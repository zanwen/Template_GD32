//
// Created by 86157 on 2024/11/14.
//

#include "hal_rtc.h"
#include "logger.h"

#define HAL_RTC_BKP_VALUE 0xFFFF

__IO uint32_t prescaler_async = 0, prescaler_ssync = 0;

// 十位取出左移4位 + 个位 (得到BCD数)
void hal_rtc_clock_write();

#define WRITE_BCD(val)    ((val / 10) << 4) + (val % 10)
// 将高4位乘以10 + 低四位 (得到10进制数)
#define READ_BCD(val)    (val >> 4) * 10 + (val & 0x0F)


void hal_rtc_init(void) {
    /* enable PMU clock */
    rcu_periph_clock_enable(RCU_PMU);
    /* enable the access of the RTC registers */
    pmu_backup_write_enable();

//    rcu_osci_on(RCU_IRC32K);
//    rcu_osci_stab_wait(RCU_IRC32K);
//    rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);
//    // 32k / (127+1) / (249+1) = 1
//    prescaler_a = 0x7F;
//    prescaler_s = 0xF9;

    //rcu_bkp_reset_enable(); // cannot work with RTC_BKP registers
    rcu_bkp_reset_disable();

    rcu_osci_on(RCU_HXTAL);
    rcu_osci_stab_wait(RCU_HXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_HXTAL_DIV_RTCDIV);
    rcu_rtc_div_config(RCU_RTC_HXTAL_DIV25); // 8M/25=320k
    // 320k / (127+1) / (2499+1) = 1
    prescaler_async = 127;
    prescaler_ssync = 2499;

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

void hal_rtc_clock_read() {
    rtc_parameter_struct ris;
    rtc_current_time_get(&ris); // 读时间
    // 没有使用BCD转换，用16进制打印
    LOG_DEBUG("%02x-%02x-%02x", ris.year, ris.month, ris.date);
    LOG_DEBUG("weekday: %02x", ris.day_of_week);
    LOG_DEBUG("%02x:%02x:%02x", ris.hour, ris.minute, ris.second);
}

void hal_rtc_clock_setup(void) {
//    if (RTC_BKP0 == HAL_RTC_BKP_VALUE) {
//        LOG_DEBUG("rtc clock has been set");
//        return;
//    }
    hal_rtc_clock_write();

}

void hal_rtc_clock_write() {
    RTC_BKP0 = HAL_RTC_BKP_VALUE;
    /* setup RTC time value */
    rtc_parameter_struct rtc_initpara;
    rtc_initpara.factor_asyn = prescaler_async;
    rtc_initpara.factor_syn = prescaler_ssync;
    rtc_initpara.year = 0x24;
    rtc_initpara.month = RTC_NOV;
    rtc_initpara.date = 0x14;
    rtc_initpara.day_of_week = RTC_THURSDAY;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    rtc_initpara.hour = 0x15;
    rtc_initpara.minute = 0x3;
    rtc_initpara.second = 0x55;
    if (rtc_init(&rtc_initpara) == SUCCESS) {
        LOG_DEBUG("set rtc clock done ")
    } else {
        LOG_ERROR("fail to set rtc clock")
    }
}

void hal_rtc_alarm_setup(void) {
    rtc_alarm_disable(RTC_ALARM0);
    rtc_alarm_struct rtc_alarm;

    // 设置闹钟掩码，使其每天触发
    rtc_alarm.alarm_mask = RTC_ALARM_DATE_MASK;

    // 由于我们使用日期掩码，这个设置不重要，但我们还是设置它
    rtc_alarm.weekday_or_date = RTC_ALARM_DATE_SELECTED;
    rtc_alarm.alarm_day = 0x00;  // 不关心具体日期

    // 设置闹钟时间为15:04:00
    rtc_alarm.alarm_hour = WRITE_BCD(15);    // 15点
    rtc_alarm.alarm_minute = WRITE_BCD(4);   // 04分
    rtc_alarm.alarm_second = WRITE_BCD(0);   // 00秒

    // 假设使用24小时制
    rtc_alarm.am_pm = RTC_AM;  // 在24小时制下，这个设置不重要

    // 配置闹钟
    rtc_alarm_config(RTC_ALARM0, &rtc_alarm);

    // 配置闹钟中断
    exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_enable(EXTI_17);
    exti_interrupt_flag_clear(EXTI_17);
    nvic_irq_enable(RTC_Alarm_IRQn, 2, 0);
    rtc_interrupt_enable(RTC_INT_ALARM0);// 启用NVIC中的RTC中断
    rtc_flag_clear(RTC_FLAG_ALRM0);

    // 开启闹钟
    rtc_alarm_enable(RTC_ALARM0);
}
