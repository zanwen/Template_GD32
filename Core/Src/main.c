#include "dri_usart0.h"
#include "int_key.h"
#include "int_led.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

void on_read_complete(uint8_t *data, uint16_t len);

int main(void) {
    systick_config();
    retarget_init(USART0);
    dri_usart0_init();
//    dri_usart0_read_complete_callabck(on_read_complete);

    uint8_t buffer[64];
    while (1) {
        if (dri_usart0_isreadable()) {
            uint16_t rbytes = dri_usart0_get_str(buffer, 64);
            LOG_DEBUG("rbytes = %d, buffer = %s", rbytes, buffer);
        }
        delay_1ms(10);
    }
}

void on_read_complete(uint8_t *data, uint16_t len) {
    LOG_DEBUG("%d %d", data[0], data[1]);
}