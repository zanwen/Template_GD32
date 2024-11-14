#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_rtc.h"

#define BUF_SIZE 100

void on_read_complete(void);

uint8_t buf[BUF_SIZE] = {0};

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    hal_rtc_init();
    hal_rtc_clock_setup();
    hal_rtc_alarm_setup();
    while (1) {
        hal_rtc_clock_read();
        delay_1ms(1000);
    }
}

void on_read_complete(void) {

}
