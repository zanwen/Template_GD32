#include "hal_usart0.h"
#include "int_key.h"
#include "int_led.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

void on_read_complete();

int main(void) {
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    while (1) {
    }
}

void on_read_complete() {
    uint8_t buffer[64];
    uint16_t rbytes = hal_usart0_get_str(buffer, 64);
    LOG_DEBUG("rbytes = %d, buffer = %s", rbytes, buffer);
}