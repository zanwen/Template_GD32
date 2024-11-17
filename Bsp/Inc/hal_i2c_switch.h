//
// Created by 86157 on 2024/11/17.
//

#ifndef HAL_I2C_SWITCH_H
#define HAL_I2C_SWITCH_H

#include "hal_i2c_soft.h"
#include "hal_i2c_hard.h"

#define HAL_I2C_SWITCH_SOFT 0
#define HAL_I2C_SWITCH_HARD 1

#define HAL_I2C_SWITCH HAL_I2C_SWITCH_HARD

#if HAL_I2C_SWITCH == HAL_I2C_SWITCH_SOFT
    #define RTC_I2C_INIT_FN() hal_i2c_soft_init()
    #define RTC_I2C_WRITE_FN(dev_addr, reg_addr, data, size) hal_i2c_soft_write(dev_addr, reg_addr, data, size)
    #define RTC_I2C_READ_FN(dev_addr, reg_addr, data, size) hal_i2c_soft_read(dev_addr, reg_addr, data, size)
#elif HAL_I2C_SWITCH == HAL_I2C_SWITCH_HARD
    #define RTC_I2C_INIT_FN() hal_i2c_hard_init()
    #define RTC_I2C_WRITE_FN(dev_addr, reg_addr, data, size) hal_i2c_hard_write(dev_addr, reg_addr, data, size)
    #define RTC_I2C_READ_FN(dev_addr, reg_addr, data, size) hal_i2c_hard_read(dev_addr, reg_addr, data, size)
#else
    #error "Invalid HAL_I2C_SWITCH value. Please choose HAL_I2C_SWITCH_SOFT or HAL_I2C_SWITCH_HARD."
#endif

#endif //HAL_I2C_SWITCH_H
