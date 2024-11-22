#include "hal_exti.h"
#include "hal_pmu.h"
#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

void on_read_complete(void);

static void delay() {
    uint32_t i, j;
    for (i = 0; i < 1000; ++i) {
        for (j = 0; j < 10000; ++j) {
            __NOP();
        }
    }
}

void hal_exti0_callback(void) {
    LOG_DEBUG("hal_exti0_callback")
}

static void GPIO_config() {
    // ³õÊ¼»¯GPIO PB2
    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_2);
}

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    //    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callback(on_read_complete);

    hal_exti_init(HAL_EXTI2, false);
    hal_pmu_init();
    GPIO_config();
    LOG_DEBUG("main init done")

    while (1) {
        gpio_bit_toggle(GPIOB, GPIO_PIN_2);
        delay();
    }
}

void on_read_complete(void) {
    uint8_t cmd = hal_usart0_get_byte();
    LOG_DEBUG("cmd = %#x", cmd)
    switch (cmd) {
        case 0x01:
            LOG_DEBUG("pmu_to_sleepmode start")
            hal_pmu_sleep();
            LOG_DEBUG("pmu_to_sleepmode stop")
            break;

        case 0x02:
            LOG_DEBUG("pmu_to_deepsleepmode start")
            hal_pmu_deepsleep();
            LOG_DEBUG("pmu_to_deepsleepmode stop")
            break;

        case 0x03:
            LOG_DEBUG("pmu_to_standbymode start")
            hal_pmu_standby();
            LOG_DEBUG("pmu_to_standbymode stop")
            break;
    }
}