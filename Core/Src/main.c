#include "dri_usart0.h"
#include "int_key.h"
#include "int_led.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

int main(void) {
    systick_config();
    dri_usart0_init();
    retarget_init(USART0);

    uint8_t buffer[64];
    while (1) {
        if (dri_usart0_isreadable()) {
            uint16_t rbytes = dri_usart0_get_str(buffer, 64);
            LOG_DEBUG("rbytes = %d, buffer = %s", rbytes, buffer);
        }
        delay_1ms(10);
    }
}