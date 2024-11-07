#include "int_key.h"
#include "int_led.h"
#include "systick.h"

void exercise1_1(void);
void exercise1_2(void);
void exercise1_3(void);
void exercise1_4(void);
void exercise2_1(void);
void exercise2_2(void);

int main(void) {
    systick_config();
    int_led_init();
    int_key_init();

//    exercise1_1();
    //    exercise1_2();
    //    exercise1_3();
    //    exercise1_4();

    //    exercise2_1();
        exercise2_2();
}

void on_key_pressed2(KEY_NO key_no) {
    uint32_t delay_ms = 250;
    switch (key_no) {
        case KEY1:
            int_led_trotting_horse_lamp(LED1, LED8, 1, delay_ms);
            break;
        case KEY2:
            int_led_trotting_horse_lamp(LED8, LED1, 1, delay_ms);
            break;
        case KEY3:
            int_led_waterfall_lamp(LED1, LED8, 1, delay_ms);
            break;
        case KEY4:
            int_led_waterfall_lamp(LED8, LED1, 1, delay_ms);
            break;
        default:
            break;
    }
}

void on_key_released2(KEY_NO key_no) {
    int_led_off_all();
}

void exercise2_2(void) {
    for (KEY_NO key_no = KEY1; key_no <= KEY4; key_no++) {
        int_key_register_callback(key_no, on_key_pressed2, on_key_released2);
    }
    while (1) {
        int_key_scan();
        delay_1ms(50);
    }
}

void on_key_pressed(KEY_NO key_no) {
    switch (key_no) {
        case KEY1:
            int_led_on(LED1);
            int_led_on(LED2);
            break;
        case KEY2:
            int_led_on(LED3);
            int_led_on(LED4);
            break;
        case KEY3:
            int_led_on(LED5);
            int_led_on(LED6);
            break;
        case KEY4:
            int_led_on(LED7);
            int_led_on(LED8);
            break;
        default:
            break;
    }
}

void on_key_released(KEY_NO key_no) {
    switch (key_no) {
        case KEY1:
            int_led_off(LED1);
            int_led_off(LED2);
            break;
        case KEY2:
            int_led_off(LED3);
            int_led_off(LED4);
            break;
        case KEY3:
            int_led_off(LED5);
            int_led_off(LED6);
            break;
        case KEY4:
            int_led_off(LED7);
            int_led_off(LED8);
            break;
        default:
            break;
    }
}

void exercise2_1(void) {
    for (KEY_NO key_no = KEY1; key_no <= KEY4; key_no++) {
        int_key_register_callback(key_no, on_key_pressed, on_key_released);
    }
    while (1) {
        int_key_scan();
        delay_1ms(50);
    }
}

/**
  - 说明：
  - 走马灯：1 亮，其它灯不亮， 到2亮时，只有2亮，其它灯不亮
  - 流水灯：1亮，到2亮时，前面的1不灭，到3亮时，前面的1、2也不灭
- 下面代码，可以写在同一个工程里，分别调用不同函数，验证结果

- 函数1：1, 3, 5, 7 走马灯亮，然后 8, 6, 4, 2 走马灯亮

- 函数2：1, 3, 5, 7 号灯依次流水点亮，然后⼀起熄灭，再 8, 6, 4, 2 号灯依次流水点亮，然后⼀起熄灭

- 函数3：1, 2, 3, 4, 5, 6, 7, 8 依次流水点亮，然后重复灭亮3次，最后全部熄灭

- 函数4：1, 2, 3, 4 一直亮着，然后5, 6, 7, 8 依次流水点亮
 */
void exercise1_1(void) {
    int32_t delay_ms = 500;
    while (1) {
        int_led_trotting_horse_lamp(LED1, LED7, 2, delay_ms);
        int_led_off_all();
        delay_1ms(delay_ms);

        int_led_trotting_horse_lamp(LED8, LED2, 2, delay_ms);
        int_led_off_all();
        delay_1ms(delay_ms);
    }
}

void exercise1_2(void) {
    int32_t delay_ms = 500;
    while (1) {
        int_led_waterfall_lamp(LED1, LED7, 2, delay_ms);
        int_led_off_all();
        delay_1ms(delay_ms);

        int_led_waterfall_lamp(LED8, LED2, 2, delay_ms);
        int_led_off_all();
        delay_1ms(delay_ms);
    }
}

void exercise1_3(void) {
    int32_t delay_ms = 500;
    int_led_waterfall_lamp(LED1, LED8, 1, delay_ms);
    for (int32_t i = 0; i < 3; ++i) {
        int_led_on_all();
        delay_1ms(delay_ms);
        int_led_off_all();
        delay_1ms(delay_ms);
    }
}

void exercise1_4(void) {
    int32_t delay_ms = 500;
    for (LED_NO led_no = LED1; led_no <= LED4; ++led_no) {
        int_led_on(led_no);
    }

    while (1) {
        int_led_waterfall_lamp(LED5, LED8, 1, delay_ms);
        for (LED_NO led_no = LED5; led_no <= LED8; ++led_no) {
            int_led_off(led_no);
        }
        delay_1ms(delay_ms);
    }
}