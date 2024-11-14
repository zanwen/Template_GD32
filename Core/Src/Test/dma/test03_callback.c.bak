#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_dma.h"

void on_read_complete(void);

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    hal_dma1_init();

    while (1) {
    }
}

uint8_t rxbuf[128] = {0};
uint8_t txbuf[128] = {0};

void on_read_complete(void) {
    hal_usart0_get_str(rxbuf, sizeof(rxbuf));
    LOG_DEBUG("receive msg from USART0: %s", rxbuf);
    LOG_DEBUG("before DMA: txbuf = %s", txbuf)
    hal_dma1_memcpy((uint32_t) rxbuf, (uint32_t) txbuf, sizeof(rxbuf));
}

void hal_dma1_callback(void) {
    LOG_DEBUG("after DMA: txbuf = %s", txbuf)
}
