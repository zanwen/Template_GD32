#include "app.h"
#include "hal_pmu.h"
#include "hal_timer.h"
#include "hal_usart0.h"
#include "retarget.h"
#include "scheduler.h"
#include "systick.h"

void on_read_complete(void);

void hal_timer5_update_callback() {
    task_update();
}

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callback(on_read_complete);
    hal_timer_basic_start(TIMER5, 1000);

    app_init();

    while (1) {
        task_execute();
    }
}

void on_read_complete(void) {

}