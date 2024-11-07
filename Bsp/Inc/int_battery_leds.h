//
// Created by 86157 on 2024/11/7.
//

#ifndef INT_BATTERY_LEDS_H
#define INT_BATTERY_LEDS_H

#include "int_led.h"

void int_battery_leds_init(uint8_t battery_level);
void int_battery_leds_start();
void int_battery_leds_update(uint8_t battery_level);
void int_battery_leds_stop();
void int_battery_leds_transform();

#endif//INT_BATTERY_LEDS_H
