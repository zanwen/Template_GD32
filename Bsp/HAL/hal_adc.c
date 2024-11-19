//
// Created by 86157 on 2024/11/19.
//

#include "hal_adc.h"
#include "systick.h"

static void rcu_config(void) {
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
}

static void adc_config(void) {
    adc_deinit();
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC contineous function disable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);

    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

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
    rcu_config();
    gpio_config();
    adc_config();
}
uint16_t hal_adc_get_value(void) {
    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while (adc_flag_get(ADC0, ADC_FLAG_EOC) == RESET)
        ;
    adc_flag_clear(ADC0, ADC_FLAG_EOC);
    return adc_routine_data_read(ADC0);
}

float hal_adc_get_internal_temp(void) {
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_16, ADC_SAMPLETIME_15);
    uint16_t adc_value = hal_adc_get_value();
    // adc / 4096 = v_temp / 3.3v
    float v_temp = adc_value * 3.3 / 4096;
    return (1.45 - v_temp) * 1000 / 4.1 + 25;
}

float hal_adc_get_potentiometer_vol(void) {
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_14, ADC_SAMPLETIME_15);
    uint16_t adc_value = hal_adc_get_value();
    // v / 3.3 = adc / 4096
    return adc_value * 3.3 / 4096;
}
