//
// Created by 86157 on 2024/11/19.
//

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "gd32f4xx.h"
#include "logger.h"

typedef struct {
    float internal_temperature;
    float potentiometer_vol;
    uint16_t ntc_temperature;
} adc_result_t;

void hal_adc_init(void);
void hal_adc_measure(adc_result_t *buf);

#endif//HAL_ADC_H
