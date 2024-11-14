//
// Created by 86157 on 2024/11/14.
//

#include "hal_rtc.h"
#include "logger.h"

__IO uint32_t prescaler_a = 0, prescaler_s = 0;


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

    rcu_bkp_reset_enable();
    rcu_bkp_reset_disable();

    rcu_osci_on(RCU_HXTAL);
    rcu_osci_stab_wait(RCU_HXTAL);
    rcu_rtc_clock_config(RCU_RTCSRC_HXTAL_DIV_RTCDIV);
    rcu_rtc_div_config(RCU_RTC_HXTAL_DIV25); // 8M/25=320k
    // 320k / (127+1) / (2499+1) = 1
    prescaler_a = 127;
    prescaler_s = 2499;

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

void hal_rtc_read() {
    rtc_parameter_struct ris;
    rtc_current_time_get(&ris); // 读时间
    // 没有使用BCD转换，用16进制打印
    LOG_DEBUG("%02x-%02x-%02x\n", ris.year, ris.month, ris.date);
    LOG_DEBUG("weekday: %02x\n", ris.day_of_week);
    LOG_DEBUG("%02x:%02x:%02x\n", ris.hour, ris.minute, ris.second);
}

void hal_rtc_write(void) {
    /* setup RTC time value */
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;
    rtc_parameter_struct   rtc_initpara;
    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x24;
    rtc_initpara.month = RTC_NOV;
    rtc_initpara.date = 0x14;
    rtc_initpara.day_of_week = RTC_THURSDAY;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    rtc_initpara.hour = 0x15;
    rtc_initpara.minute = 0x3;
    rtc_initpara.second = 0x55;
    ErrStatus status = rtc_init(&rtc_initpara);
    LOG_DEBUG("rtc_init result = %d", status)
}
