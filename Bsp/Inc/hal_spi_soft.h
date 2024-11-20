//
// Created by 86157 on 2024/11/20.
//

#ifndef HAL_SPI_SOFT_H
#define HAL_SPI_SOFT_H

#include "gd32f4xx.h"

void hal_spi_soft_init(void);
uint8_t hal_spi_soft_read(void);
void hal_spi_soft_write(uint8_t dat);

#endif//HAL_SPI_SOFT_H
