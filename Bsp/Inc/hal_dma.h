//
// Created by 86157 on 2024/11/13.
//

#ifndef HAL_DMA_H
#define HAL_DMA_H

#include "gd32f4xx.h"

void hal_dma1_m2m_init();
void hal_dma1_m2m_cpy(uint32_t src_addr, uint32_t dest_addr, uint32_t byte_size);
void hal_dma1_m2m_callback(void);

void hal_dma1_m2uart_init();
void hal_dma1_m2uart_cpy(uint32_t addr, uint32_t byte_size);
void hal_dma1_m2uart_callback(void);

void hal_dma1_usart2m_init(uint32_t buf_addr, uint32_t byte_size);
uint32_t hal_dma1_usart2m_stop();
void hal_dma1_usart2m_restart();
#endif //HAL_DMA_H
