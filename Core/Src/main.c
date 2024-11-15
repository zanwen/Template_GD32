#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_wdgt.h"
#include "int_matrixkey.h"
#include "int_nixietube.h"

#define BUF_SIZE 100

void on_read_complete(void);

void on_key_down(uint8_t row, uint8_t col);

uint8_t buf[BUF_SIZE] = {0};

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    Int_NixieTube_Init();
//    Int_NixieTube_SetStr("12345");
    uint8_t code[] = {0xF9,
                      0xA4,
                      0xB0,
                      0x99,
                      0x92,
                      0x82,
                      0xF8,
                      0x80,};

    for (uint8_t i = 0; i < 8; ++i) {
        Int_NixieTube_DisplaySingle(i, code[i]);
        delay_1ms(1000);
    }

    while (1) {
    }
}

void on_read_complete(void) {

}

void on_key_down(uint8_t row, uint8_t col) {
    LOG_DEBUG("key down: row %d, col %d\r\n", row, col);
}
