//
// Created by 86157 on 2024/11/20.
//



#ifndef HAL_SPI_INTEGRATE_H
#define HAL_SPI_INTEGRATE_H

#include "hal_spi_soft.h"
#include "hal_spi_hard.h"

#define SPI_CLK_PORT_RCU RCU_GPIOA
#define SPI_CLK_PORT GPIOA
#define SPI_CLK_PIN GPIO_PIN_5

#define SPI_MOSI_PORT_RCU RCU_GPIOA
#define SPI_MOSI_PORT GPIOA
#define SPI_MOSI_PIN GPIO_PIN_7

#define SPI_MISO_PORT_RCU RCU_GPIOA
#define SPI_MISO_PORT GPIOA
#define SPI_MISO_PIN GPIO_PIN_6

#define SPI_INTEGRATE_SOFT 0
#define SPI_INTEGRATE_HARD 1
#define SPI_INTEGRATE_MODE SPI_INTEGRATE_HARD

#if SPI_INTEGRATE_MODE == SPI_INTEGRATE_SOFT
    #define SPI_INIT() hal_spi_soft_init()
    #define SPI_WRITE(dat) hal_spi_soft_write(dat)
    #define SPI_READ() hal_spi_soft_read()
#elif SPI_INTEGRATE_MODE == SPI_INTEGRATE_HARD
    #define SPI_INIT() hal_spi_hard_init()
    #define SPI_WRITE(dat) hal_spi_hard_write(dat)
    #define SPI_READ() hal_spi_hard_read()
#endif

#endif//HAL_SPI_INTEGRATE_H
