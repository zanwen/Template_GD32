//
// Created by 86157 on 2024/11/14.
//

#include "hal_wdgt.h"

void hal_fwdgt_init(uint32_t timeout_ms) {
    // (IRC)32k / 32 = 1000 Hz = 1ms
    // timeout for 2s
    fwdgt_config(timeout_ms, FWDGT_PSC_DIV32);
    fwdgt_enable();
}

void hal_fwdgt_reload(void) {
    fwdgt_counter_reload();
}
