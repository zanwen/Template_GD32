//
// Created by 86157 on 2024/11/20.
//



#ifndef HAL_SPI_SWITCH_H
#define HAL_SPI_SWITCH_H

#include "hal_spi_soft.h"

#define SPI_WRITE(dat) hal_spi_soft_write(dat)
#define SPI_READ() hal_spi_soft_read()

#endif//HAL_SPI_SWITCH_H
