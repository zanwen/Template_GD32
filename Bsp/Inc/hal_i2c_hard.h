//
// Created by 86157 on 2024/11/16.
//

#ifndef HAL_i2c_hard_H
#define HAL_i2c_hard_H

#include "gd32f4xx.h"

void hal_i2c_hard_init();
int hal_i2c_hard_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size);
int hal_i2c_hard_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size);

#endif //HAL_i2c_hard_H
