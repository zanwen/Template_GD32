//
// Created by 86157 on 2024/11/13.
//

#include "hal_dma.h"
#include "logger.h"

#define HAL_DMA1_AND_CH DMA1, DMA_CH0

void hal_dma1_init() {
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(HAL_DMA1_AND_CH);

    dma_single_data_parameter_struct dma_init_struct;
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;

    dma_single_data_mode_init(HAL_DMA1_AND_CH, &dma_init_struct);

    nvic_irq_enable(DMA1_Channel0_IRQn, 2, 0);
    dma_interrupt_enable(HAL_DMA1_AND_CH, DMA_INT_FTF);
}

void hal_dma1_memcpy(uint32_t src, uint32_t dest, uint32_t byte_size) {
    dma_periph_address_config(HAL_DMA1_AND_CH,  src);
    dma_memory_address_config(HAL_DMA1_AND_CH, 0, dest);
    dma_transfer_number_config(HAL_DMA1_AND_CH, byte_size);
    dma_channel_enable(HAL_DMA1_AND_CH);
}

void hal_dma1_irq_handler(void) {
    if (dma_interrupt_flag_get(HAL_DMA1_AND_CH, DMA_INT_FLAG_FTF) == SET) {
        hal_dma1_callback();
        dma_interrupt_flag_clear(HAL_DMA1_AND_CH, DMA_FLAG_FTF);
    }
}

__attribute__((weak)) void hal_dma1_callback(void) {

}