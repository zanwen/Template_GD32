#include "int_nixietube.h"
#include "string.h"
#include "logger.h"
#include "systick.h"

#define NIX_DI_PORT      GPIOD
#define NIX_DI_PIN       GPIO_PIN_0
#define NIX_RCK_PORT     GPIOD
#define NIX_RCK_PIN      GPIO_PIN_1
#define NIX_SCK_PORT     GPIOD
#define NIX_SCK_PIN      GPIO_PIN_4
#define HYPHEN_INDEX 21

static uint8_t s_buffer[8];

uint8_t s_code_table[] = {
    // 0 	1	 2	-> 9	(����012...9)
    0xC0,
    0xF9,
    0xA4,
    0xB0,
    0x99,
    0x92,
    0x82,
    0xF8,
    0x80,
    0x90,
    // 0. 1. 2. -> 9.	(����10,11,12....19)
    0x40,
    0x79,
    0x24,
    0x30,
    0x19,
    0x12,
    0x02,
    0x78,
    0x00,
    0x10,
    // . -						(����20,21)
    0x7F,
    0xBF,
    // AbCdEFHJLPqU		(����22,23,24....33)
    0x88,
    0x83,
    0xC6,
    0xA1,
    0x86,
    0x8E,
    0x89,
    0xF1,
    0xC7,
    0x8C,
    0x98,
    0xC1
};


static void clear_buffer() {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        s_buffer[i] = 0;
    }
}

void Int_NixieTube_Init() {
    rcu_periph_clock_enable(RCU_GPIOD);

    gpio_mode_set(NIX_DI_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, NIX_DI_PIN);
    gpio_mode_set(NIX_RCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, NIX_RCK_PIN);
    gpio_mode_set(NIX_SCK_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, NIX_SCK_PIN);

    gpio_output_options_set(NIX_DI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, NIX_DI_PIN);
    gpio_output_options_set(NIX_RCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, NIX_RCK_PIN);
    gpio_output_options_set(NIX_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, NIX_SCK_PIN);

    clear_buffer();
}

//static void shift(uint8_t data) {
//    for (uint8_t i = 0; i < 8; ++i) {
//        gpio_bit_write(NIX_DI_PORT, NIX_DI_PIN, (data & (0x80 >> i)) ? SET : RESET);
//        gpio_bit_write(NIX_SCK_PORT, NIX_SCK_PIN, RESET);
////        __NOP();
//        gpio_bit_write(NIX_SCK_PORT, NIX_SCK_PIN, SET);
////        __NOP();
//    }
//}
//
//static void rck_action() {
//    gpio_bit_write(NIX_RCK_PORT, NIX_RCK_PIN, RESET);
////    __NOP();
//    gpio_bit_write(NIX_RCK_PORT, NIX_RCK_PIN, SET);
////    __NOP();
//}

static void shift(uint8_t data) {
    for (uint8_t i = 0; i < 8; ++i) {
        gpio_bit_write(NIX_DI_PORT, NIX_DI_PIN, (data & (0x80 >> i)) ? SET : RESET);
        delay_1us(1);
        gpio_bit_reset(NIX_SCK_PORT, NIX_SCK_PIN);
        delay_1us(1);
        gpio_bit_set(NIX_SCK_PORT, NIX_SCK_PIN);
        delay_1us(1);
    }
}

static void rck_action() {
    gpio_bit_reset(NIX_RCK_PORT, NIX_RCK_PIN);
    delay_1us(1);
    gpio_bit_set(NIX_RCK_PORT, NIX_RCK_PIN);
    delay_1us(1);
}

// chip selection: from 0 to 7
// seg_code: segment selection code
void Int_NixieTube_DisplaySingle(uint8_t chip, uint8_t seg_code) {
    shift(seg_code);
    shift(1 << chip);
    rck_action();
}

void Int_NixieTube_SetNum(long int num) {
    LOG_DEBUG("Int_NixieTube_SetNum num = %ld\n", num);

    for (int i = 0; i < 8; i++) {
        s_buffer[i] = 0;
    }

    int i = 7;
    while (num > 0) {
        s_buffer[i] = s_code_table[(num % 10)];
        LOG_DEBUG("s_buffer[%d] = %#x\n", (int)i, (int)s_buffer[i]);
        num /= 10;
        i--;
    }
}

void Int_NixieTube_Refresh() {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (s_buffer[i] > 0) {
            Int_NixieTube_DisplaySingle(i, s_buffer[i]);
            delay_1ms(1);
        }
    }
}

void Int_NixieTube_SetStr(char* str) {
    uint8_t i;
    if (str == NULL) {
        LOG_DEBUG("[Int_NixieTube_SetStr] str = null\n");
        return;
    }

    i = 0;
    while (*str) {
        if (*str >= '0' && *str <= '9') {
            s_buffer[i] = s_code_table[*str - '0'];
        } else {
            s_buffer[i] = s_code_table[HYPHEN_INDEX];
        }
        i++;
        str++;
    }
    LOG_DUMP("Int_NixieTube_SetStr done", s_buffer, 8);
}
