#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_dma.h"

void on_read_complete(void);

void hal_dma1_m2uart_callback(void) {
    LOG_DEBUG("callback, usart tx with dma done")
}

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    hal_dma1_m2uart_init();
    char buffer[100] = "hello, uart with dma!\n";
    hal_dma1_m2uart_cpy((uint32_t) buffer, strlen(buffer));

    while (1) {
    }
}

void on_read_complete(void) {

}
