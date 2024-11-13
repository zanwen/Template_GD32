//
// Created by 86157 on 2024/11/13.
//

#ifndef HAL_DMA_H
#define HAL_DMA_H

#include "gd32f4xx.h"

void hal_dma1_init();
void hal_dma1_memcpy(uint32_t src, uint32_t dest, uint32_t byte_size);
void hal_dma1_callback(void);

#endif //HAL_DMA_H
