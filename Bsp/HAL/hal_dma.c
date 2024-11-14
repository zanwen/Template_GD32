//
// Created by 86157 on 2024/11/13.
//

#include "hal_dma.h"
#include "logger.h"

#define HAL_DMA1_M2M_CH DMA1, DMA_CH0
#define HAL_DMA1_M2USART0_CH DMA1, DMA_CH7

void hal_dma1_m2m_init() {
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(HAL_DMA1_M2M_CH);

    dma_single_data_parameter_struct dma_init_struct;
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.priority = DMA_PRIORITY_LOW;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;

    dma_single_data_mode_init(HAL_DMA1_M2M_CH, &dma_init_struct);

    nvic_irq_enable(DMA1_Channel0_IRQn, 2, 0);
    dma_interrupt_enable(HAL_DMA1_M2M_CH, DMA_INT_FTF);
}

void hal_dma1_m2m_cpy(uint32_t src_addr, uint32_t dest_addr, uint32_t byte_size) {
    dma_periph_address_config(HAL_DMA1_M2M_CH, src_addr);
    dma_memory_address_config(HAL_DMA1_M2M_CH, 0, dest_addr);
    dma_transfer_number_config(HAL_DMA1_M2M_CH, byte_size);
    dma_channel_enable(HAL_DMA1_M2M_CH);
}

void hal_dma1_irq_handler(void) {
    if (dma_interrupt_flag_get(HAL_DMA1_M2M_CH, DMA_INT_FLAG_FTF) == SET) {
        hal_dma1_m2m_callback();
        dma_interrupt_flag_clear(HAL_DMA1_M2M_CH, DMA_FLAG_FTF);
    }
}

__attribute__((weak)) void hal_dma1_m2m_callback(void) {

}

void hal_dma1_m2uart_init() {
    rcu_periph_clock_enable(RCU_DMA1);
    dma_deinit(HAL_DMA1_M2USART0_CH);

    dma_single_data_parameter_struct dma_init_struct;
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_MEMORY_INCREASE_DISABLE;
    dma_init_struct.memory_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_HIGH;
    dma_init_struct.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(HAL_DMA1_M2USART0_CH, &dma_init_struct);

    usart_dma_transmit_config(USART0, USART_TRANSMIT_DMA_ENABLE);
    dma_channel_subperipheral_select(HAL_DMA1_M2USART0_CH, DMA_SUBPERI4);
}

void hal_dma1_m2uart_cpy(uint32_t addr, uint32_t byte_size) {
    dma_memory_address_config(HAL_DMA1_M2USART0_CH, DMA_MEMORY_0, addr);
    dma_transfer_number_config(HAL_DMA1_M2USART0_CH, byte_size);
    dma_channel_enable(HAL_DMA1_M2USART0_CH);
}
