//
// Created by 86157 on 2024/11/19.
//

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "gd32f4xx.h"
#include "logger.h"

void hal_adc_init(void);
uint16_t hal_adc_get_value(void);
float hal_adc_get_internal_temp(void);
float hal_adc_get_potentiometer_vol(void);

#endif//HAL_ADC_H
