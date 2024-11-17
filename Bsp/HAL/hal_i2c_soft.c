//
// Created by 86157 on 2024/11/16.
//
#include <stdbool.h>
#include "hal_i2c_soft.h"
#include "systick.h"
#include "logger.h"

#define I2C_SCL_PIN    GPIO_PIN_6
#define I2C_SDA_PIN    GPIO_PIN_7
#define I2C_GPIO_PORT  GPIOB
#define I2C_GPIO_RCU   RCU_GPIOB

#define SDA_HIGH() gpio_bit_set(I2C_GPIO_PORT, I2C_SDA_PIN)

#define SDA_LOW() gpio_bit_reset(I2C_GPIO_PORT, I2C_SDA_PIN)

#define SCL_HIGH() gpio_bit_set(I2C_GPIO_PORT, I2C_SCL_PIN)

#define SCL_LOW() gpio_bit_reset(I2C_GPIO_PORT, I2C_SCL_PIN)

#define SDA_IN() gpio_mode_set(I2C_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, I2C_SDA_PIN)
#define SDA_OUT() gpio_mode_set(I2C_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C_SDA_PIN)
#define SDA_READ() gpio_input_bit_get(I2C_GPIO_PORT, I2C_SDA_PIN)

void hal_i2c_soft_init() {
    // 使能GPIOB时钟
    rcu_periph_clock_enable(I2C_GPIO_RCU);

    // 配置PB6（SCL）和PB7（SDA）为开漏输出模式
    gpio_mode_set(I2C_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, I2C_SCL_PIN | I2C_SDA_PIN);
    gpio_output_options_set(I2C_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, I2C_SCL_PIN | I2C_SDA_PIN);

    // 将SCL和SDA线拉高
    gpio_bit_set(I2C_GPIO_PORT, I2C_SCL_PIN | I2C_SDA_PIN);
}

static void start() {
    SDA_HIGH();
    SCL_HIGH();
    delay_1us(1); //start setup
    SDA_LOW();
    delay_1us(1); //start hold

    SCL_LOW();
    delay_1us(1);
}

static void stop() {
    SDA_LOW();
    SCL_HIGH();
    delay_1us(1); //stop setup
    SDA_HIGH();
    delay_1us(2); //bus free time between a STOP and START condition
}

static bool wait_ack() {
    bool success = false;
    SDA_HIGH(); // release bus
    SCL_HIGH();
    delay_1us(1); // scl high

    SDA_IN();
    success = SDA_READ() == RESET; // read ack
    SDA_OUT();

    SCL_LOW();
    delay_1us(1); // scl low
    return success;
}

static void send_ack(bool ack) {
    // set sda to reponse ack/nack
    if (ack) {
        SDA_LOW();
    } else {
        SDA_HIGH();
    }
    delay_1us(1); // sda setup

    SCL_HIGH(); // let slave read ack
    delay_1us(1);

    SCL_LOW();
    delay_1us(1); // reset scl for next operation
}

static void send_byte(uint8_t byte) {
    for (int i = 0; i < 8; ++i) {
        if (byte & (0x80 >> i)) {
            SDA_HIGH();
        } else {
            SDA_LOW();
        }
        delay_1us(1); // data set-up

        SCL_HIGH();
        delay_1us(1); // scl high

        SCL_LOW();
        delay_1us(1);
    }
}

static void receive_byte(uint8_t *buf) {
    SDA_IN();
    for (int i = 0; i < 8; ++i) {
        SCL_HIGH(); // slave keep data
        delay_1us(1);

        // read data
        (*buf) <<= 1;
        if(SDA_READ() == SET) {
            (*buf) |= 0x01;
        }

        SCL_LOW(); // let slave prepare next data
        delay_1us(1);
    }
    SDA_OUT();
}

static bool start_for_write(uint8_t dev_addr, uint8_t reg_addr) {
    start();

    send_byte(dev_addr);
    if (!wait_ack()) {
        stop();
        LOG_DEBUG("write dev_addr failed")
        return false;
    }

    send_byte(reg_addr);
    if (!wait_ack()) {
        stop();
        LOG_DEBUG("write reg_addr failed")
        return false;
    }
    return true;
}

static bool start_for_read(uint8_t dev_addr) {
    start();
    send_byte(dev_addr | 0x01); // read from slave
    if (!wait_ack()) {
        stop();
        LOG_DEBUG("write dev_addr for read failed")
        return false;
    }
    return true;
}

bool hal_i2c_soft_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t size) {
    if (!start_for_write(dev_addr, reg_addr)) {
        return false;
    }
    for (uint16_t i = 0; i < size; ++i) {
        send_byte(*data);
        if (!wait_ack()) {
            stop();
            LOG_DEBUG("write data[%d] failed", *data)
            return false;
        }
        data++;
    }
    stop();
    return true;
}

bool hal_i2c_soft_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *buf, uint16_t size) {
    // dummy write for register address
    if (!start_for_write(dev_addr, reg_addr)) {
        return false;
    }
    // continuous read
    if (!start_for_read(dev_addr)) {
        return false;
    }
    for (uint16_t i = 0; i < size - 1; ++i) {
        receive_byte(buf++);
        send_ack(true);
    }
    receive_byte(buf);
    send_ack(false);

    stop();
    return true;
}

