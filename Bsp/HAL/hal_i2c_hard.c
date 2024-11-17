//
// Created by 86157 on 2024/11/16.
//
#include <stdbool.h>
#include "hal_i2c_hard.h"
#include "systick.h"
#include "logger.h"

void hal_i2c_hard_init() {

}

int hal_i2c_hard_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size) {

    return true;
}

int hal_i2c_hard_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size) {

    return true;
}

