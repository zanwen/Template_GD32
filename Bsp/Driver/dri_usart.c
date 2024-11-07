//
// Created by 86157 on 2024/11/6.
//
#include "../Inc/dri_usart.h"
#include "gd32f4xx.h"

#define USART0_RCU_GPIO_PORT RCU_GPIOA
#define USART0_TX_PORT GPIOA
#define USART0_TX_PIN GPIO_PIN_9
#define USART0_RX_PORT GPIOA
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_GPIO_AF GPIO_AF_7

static void configure_usart0_gpio(void);
static void configure_usart0(void);


void dri_USART0_init(void) {
    // 1. Enable clocks
    rcu_periph_clock_enable(USART0_RCU_GPIO_PORT);
    rcu_periph_clock_enable(RCU_USART0);

    // 2. Configure GPIO pins
    configure_usart0_gpio();

    // 3. Configure USART
    configure_usart0();

    // 4. Enable USART
    usart_enable(USART0);
}

static void configure_usart0_gpio(void) {
    // Configure TX pin
    gpio_af_set(USART0_TX_PORT, USART0_GPIO_AF, USART0_TX_PIN);
    gpio_mode_set(USART0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_TX_PIN);
    gpio_output_options_set(USART0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_TX_PIN);

    // Configure RX pin
    gpio_af_set(USART0_RX_PORT, USART0_GPIO_AF, USART0_RX_PIN);
    gpio_mode_set(USART0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_RX_PIN);
}

static void configure_usart0(void) {
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_data_first_config(USART0, USART_MSBF_LSB);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
}
void dri_USART_send_byte(uint32_t usart_periph, uint8_t data) {
    usart_data_transmit(usart_periph, data);
    while(RESET == usart_flag_get(usart_periph, USART_FLAG_TBE));
}
void dri_USART_send_bytes(uint32_t usart_periph, uint8_t *data, uint32_t len) {
    if (data) {
        while (len--) {
            dri_USART_send_byte(usart_periph, *data++);
        }
    }
}
void dri_USART_send_str(uint32_t usart_periph, char *str) {
    if (str) {
        while (*str) {
            dri_USART_send_byte(usart_periph, *str);
            str++;
        }
    }
}
