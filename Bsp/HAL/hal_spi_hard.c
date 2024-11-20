//
// Created by 86157 on 2024/11/20.
//

#include "hal_spi_hard.h"
#include "hal_spi_integrate.h"
#include "logger.h"

static void gpio_config() {
    rcu_periph_clock_enable(SPI_CLK_PORT_RCU);
    rcu_periph_clock_enable(SPI_MOSI_PORT_RCU);
    rcu_periph_clock_enable(SPI_MISO_PORT_RCU);

    gpio_mode_set(SPI_CLK_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_CLK_PIN);
    gpio_mode_set(SPI_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MOSI_PIN);
    gpio_mode_set(SPI_MISO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MISO_PIN);
    gpio_af_set(SPI_CLK_PORT, GPIO_AF_5, SPI_CLK_PIN);
    gpio_af_set(SPI_CLK_PORT, GPIO_AF_5, SPI_MOSI_PIN);
    gpio_af_set(SPI_CLK_PORT, GPIO_AF_5, SPI_MISO_PIN);

    gpio_output_options_set(SPI_CLK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_CLK_PIN);
    gpio_output_options_set(SPI_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, SPI_MOSI_PIN);

    gpio_bit_set(SPI_CLK_PORT, SPI_CLK_PIN);
    gpio_bit_set(SPI_MOSI_PORT, SPI_MOSI_PIN);
}

static void spi_config() {
    rcu_periph_clock_enable(RCU_SPI0);

    spi_i2s_deinit(SPI0);
    spi_parameter_struct spi_struct;
    spi_struct_para_init(&spi_struct);
    spi_struct.device_mode = SPI_MASTER;
    spi_struct.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_struct.frame_size = SPI_FRAMESIZE_8BIT;
    spi_struct.nss = SPI_NSS_SOFT;
    spi_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_struct.prescale = SPI_PSC_2;
    spi_struct.endian = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_struct);

    spi_enable(SPI0);
}

void hal_spi_hard_init(void) {
    gpio_config();
    spi_config();
    LOG_DEBUG("hal_spi_hard_init done")
}
uint8_t hal_spi_hard_read(void) {
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET);
    spi_i2s_data_transmit(SPI0, 0);
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET);
    return spi_i2s_data_receive(SPI0);
}

void hal_spi_hard_write(uint8_t dat) {
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_TBE) == RESET);
    spi_i2s_data_transmit(SPI0, dat);
    while (spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE) == RESET);
    spi_i2s_data_receive(SPI0);
}
