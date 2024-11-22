//
// Created by 86157 on 2024/11/22.
//

#include "hal_pmu.h"
void hal_pmu_init(void) {
    rcu_periph_clock_enable(RCU_PMU);
}
void hal_pmu_sleep(void) {
    pmu_to_sleepmode(WFI_CMD);
}
void hal_pmu_deepsleep(void) {
    pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, PMU_LOWDRIVER_ENABLE, WFI_CMD);
    SystemInit();
}
void hal_pmu_standby(void) {
    pmu_flag_clear(PMU_FLAG_RESET_STANDBY);
    pmu_wakeup_pin_enable();
    pmu_to_standbymode();
}
