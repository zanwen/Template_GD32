#include "int_key.h"
#include "systick.h"
#include "int_battery_leds.h"

uint8_t battery_level = 1;
void on_key_pressed(KEY_NO key_no);
void on_key_released(KEY_NO key_no);

int main(void) {
    systick_config();
    int_key_init();
    int_battery_leds_init(battery_level);

    for (KEY_NO key_no = KEY1; key_no <= KEY4; key_no++) {
        int_key_register_callback(key_no, on_key_pressed, on_key_released);
    }
    while (1) {
        int_key_scan();
        int_battery_leds_transform();
        delay_1ms(250);
    }
}

void on_key_pressed(KEY_NO key_no) {
    uint32_t delay_ms = 250;
    switch (key_no) {
        case KEY1:
            int_battery_leds_start();
            break;
        case KEY2:
            int_battery_leds_stop();
            break;
        case KEY3:
            int_battery_leds_update(++battery_level);
            break;
        default:
            break;
    }
}

void on_key_released(KEY_NO key_no) {

}
