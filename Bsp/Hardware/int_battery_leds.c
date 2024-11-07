//
// Created by 86157 on 2024/11/7.
//

#include "int_battery_leds.h"
#include "systick.h"

typedef enum {
    BATTERY_DEFAULT,
    BATTERY_CHARGING,
    BATTERY_STOP_CHARGING,
} battery_state;

static uint8_t sg_battery_level;
static LED_INDEX sg_led_horse_index;
static battery_state sg_battery_state;

void int_battery_leds_init(uint8_t battery_level) {
    sg_battery_state = BATTERY_DEFAULT;
    int_led_init();
    int_battery_leds_update(battery_level);
}

void int_battery_leds_start() {
    sg_battery_state = BATTERY_CHARGING;
}

void int_battery_leds_update(uint8_t battery_level) {
    if (battery_level < LED_SIZE) {
        sg_battery_level = battery_level;
        sg_led_horse_index = sg_battery_level - 1;
    }
}

void int_battery_leds_stop() {
    sg_battery_state = BATTERY_STOP_CHARGING;
}

void int_battery_leds_transform() {
    switch (sg_battery_state) {
        case BATTERY_DEFAULT:
            int_led_off_all(0);
            break;
        case BATTERY_CHARGING:
            for (LED_INDEX led_index = LED1; led_index <= LED_SIZE; led_index++) {
                bit_status status = led_index <= sg_led_horse_index ? LED_ON : LED_OFF;
                int_led_turn(led_index, status, 0);
            }
            sg_led_horse_index++;
            if (sg_led_horse_index >= LED_SIZE) {
                sg_led_horse_index = sg_battery_level - 1;
            }
            break;
        case BATTERY_STOP_CHARGING:
            for (uint8_t i = 0; i < 3; ++i) {
                int_led_off_all(250);
                for (LED_INDEX led_index = LED1; led_index < sg_battery_level; led_index++) {
                    int_led_on(led_index, 0);
                }
                delay_1ms(250);
            }
            sg_battery_state = BATTERY_DEFAULT;
            break;
        default:
            break;
    }
}