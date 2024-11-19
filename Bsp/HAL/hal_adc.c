//
// Created by 86157 on 2024/11/19.
//

#include "hal_adc.h"
#include "systick.h"

#define ADC_CHANNEL_SIZE 2
#define ADC_RANK_POTENTIOMETER 0
#define ADC_RANK_TEMPERATURE 1
#define ADC_DMA_CHANNEL DMA1, DMA_CH0

static uint16_t adc_value[ADC_CHANNEL_SIZE] = {0};

static void dma_config(void) {
    rcu_periph_clock_enable(RCU_DMA1);
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(ADC_DMA_CHANNEL);

    /* initialize DMA single data mode */
    dma_single_data_parameter.periph_addr = (uint32_t) (&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t) (adc_value);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = ADC_CHANNEL_SIZE;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(ADC_DMA_CHANNEL, &dma_single_data_parameter);
    dma_channel_subperipheral_select(ADC_DMA_CHANNEL, DMA_SUBPERI0);

    /* enable DMA circulation mode */
    dma_circulation_enable(ADC_DMA_CHANNEL);

    /* enable DMA channel */
    dma_channel_enable(ADC_DMA_CHANNEL);
}

static void adc_config(void) {
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);

    adc_deinit();
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC contineous function disable */
    // trigger manually by software
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_CHANNEL_SIZE);
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_14, ADC_SAMPLETIME_15);
    adc_routine_channel_config(ADC0, 1, ADC_CHANNEL_16, ADC_SAMPLETIME_15);

    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC0);
    adc_dma_mode_enable(ADC0);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}

static void gpio_config() {
    // PC4 potentiometer, analog input
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);
}

void hal_adc_init(void) {
    gpio_config();
    dma_config();
    adc_config();
}
void hal_adc_trigger(void) {
    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while (dma_flag_get(ADC_DMA_CHANNEL, DMA_FLAG_FTF) == RESET)
        ;
    dma_flag_clear(ADC_DMA_CHANNEL, DMA_FLAG_FTF);
}

float hal_adc_get_internal_temp(void) {
    hal_adc_trigger();
    uint16_t adc = adc_value[ADC_RANK_TEMPERATURE];
    // adc / 4096 = v_temp / 3.3v
    float v_temp = adc * 3.3 / 4096;
    return (1.45 - v_temp) * 1000 / 4.1 + 25;
}

float hal_adc_get_potentiometer_vol(void) {
    hal_adc_trigger();
    uint16_t adc = adc_value[ADC_RANK_POTENTIOMETER];
    // v / 3.3 = adc / 4096
    return adc * 3.3 / 4096;
}
