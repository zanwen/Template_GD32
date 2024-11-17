//
// Created by 86157 on 2024/11/16.
//
#include <stdbool.h>
#include "hal_i2c_hard.h"
#include "systick.h"
#include "logger.h"

// I2C 硬件相关宏定义
#define I2C_PORT                GPIOB
#define I2C_SCL_PIN             GPIO_PIN_6
#define I2C_SDA_PIN             GPIO_PIN_7
#define I2C_GPIO_RCU            RCU_GPIOB
#define I2C_RCU                 RCU_I2C0
#define I2C_PERIPH              I2C0
#define I2C_GPIO_AF             GPIO_AF_4

#define I2C_SPEED               400000  // 100kHz
#define I2C_DUTY_CYCLE          I2C_DTCY_2

#define I2C_TIMEOUT             100000  // 超时时间

void hal_i2c_hard_init() {
    // 使能 GPIO 时钟
    rcu_periph_clock_enable(I2C_GPIO_RCU);

    // 使能 I2C 时钟
    rcu_periph_clock_enable(I2C_RCU);

    // 配置 SCL 和 SDA 为复用功能模式
    gpio_mode_set(I2C_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, I2C_SCL_PIN | I2C_SDA_PIN);

    // 设置引脚为开漏输出
    gpio_output_options_set(I2C_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN | I2C_SDA_PIN);

    // 设置引脚复用为 I2C 功能
    gpio_af_set(I2C_PORT, I2C_GPIO_AF, I2C_SCL_PIN | I2C_SDA_PIN);

    // I2C 配置
    i2c_deinit(I2C_PERIPH);
    i2c_clock_config(I2C_PERIPH, I2C_SPEED, I2C_DUTY_CYCLE);
    i2c_mode_addr_config(I2C_PERIPH, I2C_I2CMODE_ENABLE, I2C_ADDFORMAT_7BITS, 0x00); // 配置为主模式
    i2c_enable(I2C_PERIPH);
    // must after I2CEN
    i2c_ack_config(I2C_PERIPH, I2C_ACK_ENABLE);
}

static bool wait_for_flag(uint32_t flag, FlagStatus status) {
    uint32_t timeout = I2C_TIMEOUT;
    while (i2c_flag_get(I2C_PERIPH, flag) != status) {
        if (--timeout == 0) {
            LOG_ERROR("wait for flag [%lu => %d] timeout", flag, status);
            return false;
        }
    }
    return true;
}

static bool send_start() {
    i2c_start_on_bus(I2C_PERIPH);
    // start bid send
    return wait_for_flag(I2C_FLAG_SBSEND, SET);
}

static bool send_device_address(uint8_t dev_addr, uint32_t direction) {
    i2c_master_addressing(I2C_PERIPH, dev_addr, direction);
    if (!wait_for_flag(I2C_FLAG_ADDSEND, SET)) return false;
    i2c_flag_clear(I2C_PERIPH, I2C_FLAG_ADDSEND);
    return true;
}

static bool send_byte(uint8_t byte) {
    i2c_data_transmit(I2C_PERIPH, byte);
    return wait_for_flag(I2C_FLAG_TBE, SET);
}

static bool send_stop() {
    i2c_stop_on_bus(I2C_PERIPH);
    return wait_for_flag(I2C_CTL0_STOP, RESET);
}

int hal_i2c_hard_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size) {
    // 等待I2C总线空闲
    if (!wait_for_flag(I2C_FLAG_I2CBSY, RESET)) return false;

    // 发送起始信号
    if (!send_start()) return false;

    // 发送设备地址（写模式）
    if (!send_device_address(dev_addr, I2C_TRANSMITTER)) return false;

    // 发送寄存器地址
    if (!send_byte(reg_addr)) return false;

    // 发送数据
    for (uint16_t i = 0; i < size; i++) {
        if (!send_byte(data[i])) return false;
    }

    // 发送停止信号
    return send_stop();
}

int hal_i2c_hard_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size) {
    // 等待I2C总线空闲
    if (!wait_for_flag(I2C_FLAG_I2CBSY, RESET)) return false;

    // 发送起始信号
    if (!send_start()) return false;

    // 发送设备地址（写模式）
    if (!send_device_address(dev_addr, I2C_TRANSMITTER)) return false;

    // 发送寄存器地址
    if (!send_byte(reg_addr)) return false;

    // 重新发送起始信号（重复起始）
    if (!send_start()) return false;

    // 发送设备地址（读模式）
    if (!send_device_address(dev_addr, I2C_RECEIVER)) return false;

    // 接收数据
    i2c_ackpos_config(I2C_PERIPH, I2C_ACKPOS_CURRENT);
    if (!wait_for_flag(I2C_CTL0_ACKEN, SET)) return false;
    for (uint16_t i = 0; i < size; i++) {
        if (i == size - 1) {
            i2c_ack_config(I2C_PERIPH, I2C_ACK_DISABLE);  // 最后一个字节发送NACK
        }
        if (!wait_for_flag(I2C_FLAG_RBNE, SET)) return false;
        buf[i] = i2c_data_receive(I2C_PERIPH);
    }

    // 发送停止信号
    if (!send_stop()) return false;

    i2c_ack_config(I2C_PERIPH, I2C_ACK_ENABLE);  // 恢复ACK配置

    return true;
}