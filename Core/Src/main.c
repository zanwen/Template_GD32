#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "int_buzzer.h"

void on_read_complete(void);

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    int_buzzer_init();

    while (1) {
        delay_1ms(1000);
    }
}

void on_read_complete(void) {
    uint8_t cmd = hal_usart0_get_byte();
    static uint8_t duty_cycle = 0;
    LOG_DEBUG("cmd: %#x", cmd)
    if (cmd == 0x01) {
//        int_buzzer_buzz(1000,500);
//        int_buzzer_stop();
        int_buzzer_demo();
    }
}

