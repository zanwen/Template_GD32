#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_wdgt.h"
#include "int_matrixkey.h"
#include "int_nixietube.h"
#include "int_dht11.h"
#include "hal_i2c_soft.h"

#define DEV_ADDR 0xA2
#define REG_ADDR_SECOND 0x02

void on_read_complete(void);

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    hal_i2c_soft_init();
    uint8_t second = 0x10;
    hal_i2c_soft_write(DEV_ADDR, REG_ADDR_SECOND, &second, 1);

    second = 0;
    while (1) {
        hal_i2c_soft_read(DEV_ADDR, REG_ADDR_SECOND, &second, 1);
        LOG_DEBUG("Second: %#x", second);
        delay_1ms(1000);
    }
}

void on_read_complete(void) {

}
