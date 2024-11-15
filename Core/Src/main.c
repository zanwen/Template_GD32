#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_wdgt.h"
#include "int_matrixkey.h"

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

    Int_MatrixKey_Init();
    Int_MatrixKey_RegisterKeyDownCallabck(on_key_down);

    while (1) {
        Int_MatrixKey_Detect();
        delay_1ms(10);
    }
}

void on_read_complete(void) {

}

void on_key_down(uint8_t row, uint8_t col) {
    LOG_DEBUG("key down: row %d, col %d\r\n", row, col);
}
