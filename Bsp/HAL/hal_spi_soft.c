//
// Created by 86157 on 2024/11/20.
//
#include "hal_spi_soft.h"

#define SPI_CLK_PORT_RCU RCU_GPIOA
#define SPI_CLK_PORT GPIOA
#define SPI_CLK_PIN GPIO_PIN_5

#define SPI_MOSI_PORT_RCU RCU_GPIOA
#define SPI_MOSI_PORT GPIOA
#define SPI_MOSI_PIN GPIO_PIN_7

#define SPI_MISO_PORT_RCU RCU_GPIOA
#define SPI_MISO_PORT GPIOA
#define SPI_MISO_PIN GPIO_PIN_6

#define SPI_SCL_RESET() gpio_bit_reset(SPI_CLK_PORT, SPI_CLK_PIN)//SCL
#define SPI_SCL_SET() gpio_bit_set(SPI_CLK_PORT, SPI_CLK_PIN)

#define SPI_MOSI_RESET() gpio_bit_reset(SPI_MOSI_PORT, SPI_MOSI_PIN)//MOSI
#define SPI_MOSI_SET() gpio_bit_set(SPI_MOSI_PORT, SPI_MOSI_PIN)

#define SPI_MISO_READ() gpio_input_bit_get(SPI_MISO_PORT, SPI_MISO_PIN)//MISO

void hal_spi_soft_init(void) {
    rcu_periph_clock_enable(SPI_CLK_PORT_RCU);
    rcu_periph_clock_enable(SPI_MOSI_PORT_RCU);
    rcu_periph_clock_enable(SPI_MISO_PORT_RCU);

    gpio_mode_set(SPI_CLK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_CLK_PIN);
    gpio_output_options_set(SPI_CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_CLK_PIN);

    gpio_mode_set(SPI_MOSI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_MOSI_PIN);
    gpio_output_options_set(SPI_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_MOSI_PIN);

    gpio_mode_set(SPI_MISO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SPI_MISO_PIN);

    gpio_bit_set(SPI_CLK_PORT, SPI_CLK_PIN);
    gpio_bit_set(SPI_MOSI_PORT, SPI_MOSI_PIN);
}

uint8_t hal_spi_soft_read(void) {
    uint8_t i, read = 0;
    for (i = 0; i < 8; i++) {
        SPI_SCL_RESET();
        read <<= 1;
        if (SPI_MISO_READ()) {
            read++;
        }
        SPI_SCL_SET();
    }
    return read;
}

void hal_spi_soft_write(uint8_t dat) {
    for (uint8_t i = 0; i < 8; i++) {
        SPI_SCL_RESET();
        if (dat & 0x80)
            SPI_MOSI_SET();
        else
            SPI_MOSI_RESET();
        SPI_SCL_SET();
        dat <<= 1;
    }
}
