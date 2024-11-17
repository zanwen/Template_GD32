//
// Created by 86157 on 2024/11/16.
//

#ifndef HAL_I2C_SOFT_H
#define HAL_I2C_SOFT_H

#include "gd32f4xx.h"
#include <stdbool.h>

void hal_i2c_soft_init();
bool hal_i2c_soft_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size);
bool hal_i2c_soft_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size);
void hal_i2c_soft_test();

#endif //HAL_I2C_SOFT_H
