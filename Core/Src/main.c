#include "hal_exti.h"
#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"

void on_read_complete(void);

int main(void) {
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);
    hal_exti_init(HAL_EXTI0, false);
    hal_exti_init(HAL_EXTI2, false);

    while (1) {
    }
}

void on_read_complete(void) {
    uint8_t buffer[64];
    uint16_t rbytes = hal_usart0_get_str(buffer, 64);
    LOG_DEBUG("rbytes = %d, buffer = %s", rbytes, buffer);
}

void hal_exti0_callback(void) {
    static uint64_t start, elapsed;
    if (gpio_input_bit_get(HAL_EXTI0_GPIO_PORT, GPIO_PIN_0) == RESET) {
        start = get_systick_us();
    } else {
        // rising edge - falling edge
        elapsed = (get_systick_us() - start) / 1000;
        LOG_DEBUG("elapsed: %llu ms", elapsed);
        if (elapsed > 200 && elapsed < 2000) {
            LOG_DEBUG("short press");
        } else if (elapsed > 2000) {
            LOG_DEBUG("long press");
        }
    }
}

void hal_exti2_callback(void) {
    LOG_DEBUG("hal_exti2_callback");
}
