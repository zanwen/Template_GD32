//
// Created by 86157 on 2024/11/6.
//
#include "hal_usart0.h"
#include "com_util.h"
#include "gd32f4xx.h"
#include "logger.h"

#define USART0_RCU_GPIO_PORT RCU_GPIOA
#define USART0_TX_PORT GPIOA
#define USART0_TX_PIN GPIO_PIN_9
#define USART0_RX_PORT GPIOA
#define USART0_RX_PIN GPIO_PIN_10
#define USART0_GPIO_AF GPIO_AF_7

#define RX_BUFFER_SIZE 512

static const uint16_t sg_rxbuf_size = RX_BUFFER_SIZE;
static uint8_t sg_rxbuf[RX_BUFFER_SIZE + 1];
static __IO uint16_t sg_rxbuf_rindex = 0;
static __IO uint16_t sg_rxbuf_windex = 0;
static callback_t sg_read_callback = NULL;

void hal_usart0_init(void) {
    // 1. Enable clocks
    rcu_periph_clock_enable(USART0_RCU_GPIO_PORT);
    rcu_periph_clock_enable(RCU_USART0);

    // 2. Configure GPIO pins
    // Configure TX pin
    gpio_af_set(USART0_TX_PORT, USART0_GPIO_AF, USART0_TX_PIN);
    gpio_mode_set(USART0_TX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_TX_PIN);
    // inner driving mode
    gpio_output_options_set(USART0_TX_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, USART0_TX_PIN);
    // Configure RX pin
    gpio_af_set(USART0_RX_PORT, USART0_GPIO_AF, USART0_RX_PIN);
    gpio_mode_set(USART0_RX_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART0_RX_PIN);

    // 3. Configure USART
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_data_first_config(USART0, USART_MSBF_LSB);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);

    // 4. Enable USART
    usart_enable(USART0);

    // 5. Enable USART interrupt
    nvic_irq_enable(USART0_IRQn, 1, 0);
    usart_interrupt_enable(USART0, USART_INT_RBNE);
    usart_interrupt_enable(USART0, USART_INT_IDLE);
}

void hal_usart0_send_byte(uint8_t data) {
    usart_data_transmit(USART0, data);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
        ;
}

void hal_usart0_send_bytes(uint8_t data[], uint32_t len) {
    if (data) {
        while (len--) {
            hal_usart0_send_byte(*data++);
        }
    }
}

void hal_usart0_send_str(char *str) {
    if (str) {
        while (*str) {
            hal_usart0_send_byte(*str);
            str++;
        }
    }
}

uint8_t hal_usart0_get_byte(void) {
    uint8_t data = 0;
    if (hal_usart0_isreadable()) {
        data = sg_rxbuf[sg_rxbuf_rindex++];
        if (sg_rxbuf_rindex >= RX_BUFFER_SIZE) {
            sg_rxbuf_rindex = 0;
        }
    }
    return data;
}

void hal_usart0_handle_irq(void) {
    if ((usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) == SET) &&
        (usart_flag_get(USART0, USART_FLAG_RBNE) == SET)) {
        // clear interrupt flag
        usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);
        uint16_t byte_data = usart_data_receive(USART0);
        // check buffer free space
        if ((sg_rxbuf_windex + 1) % RX_BUFFER_SIZE == sg_rxbuf_rindex) {
            // buffer is full, discard the new data
            LOG_ERROR("[hal_usart0_handle_irq] buffer is full, discard the new data 0x%02x. "
                      "windex = %d, rindex = %d\r\n",
                      byte_data, sg_rxbuf_windex, sg_rxbuf_rindex);
            return;
        }
        // write data and move windex
        sg_rxbuf[sg_rxbuf_windex++] = usart_data_receive(USART0);
        if (sg_rxbuf_windex >= RX_BUFFER_SIZE) {
            sg_rxbuf_windex = 0;
        }
    }

    if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE) == SET) {
        // clear interrupt flag by usart_data_receive
        usart_data_receive(USART0);
        LOG_DEBUG("USART_IT_IDLE event");
        if (sg_read_callback) {
            sg_read_callback();
            sg_rxbuf_rindex = sg_rxbuf_windex = 0;
        }
    }
}
bool hal_usart0_isreadable(void) {
    return (sg_rxbuf_rindex != sg_rxbuf_windex);
}

uint16_t hal_usart0_read(uint8_t *buf, uint16_t bufsize) {
    if (!hal_usart0_isreadable()) {
        return 0;
    }
    uint16_t readable_size = (sg_rxbuf_windex + sg_rxbuf_size - sg_rxbuf_rindex) % sg_rxbuf_size;
    uint16_t read_size = com_util_min(readable_size, bufsize);
    for (uint16_t i = 0; i < read_size; i++) {
        buf[i] = sg_rxbuf[sg_rxbuf_rindex++];
        if (sg_rxbuf_rindex >= sg_rxbuf_size) {
            sg_rxbuf_rindex = 0;
        }
    }
    return read_size;
}
uint16_t hal_usart0_get_str(uint8_t *buf, uint16_t bufsize) {
    if (bufsize == 0) {
        return 0;
    }
    uint16_t read_size = hal_usart0_read(buf, bufsize - 1);
    buf[read_size] = '\0';
    return read_size;
}
void hal_usart0_read_complete_callabck(callback_t callback) {
    sg_read_callback = callback;
}
