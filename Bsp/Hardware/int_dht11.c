#include "int_dht11.h"
#include "logger.h"
#include "systick.h"

#define DHT_RCU_GPIO RCU_GPIOB
#define DHT_GPIO_PORT GPIOB
#define DHT_GPIO_PIN GPIO_PIN_15
#define DHT_GPIO_PROT_PIN DHT_GPIO_PORT, DHT_GPIO_PIN
#define DHT_SET gpio_bit_set(DHT_GPIO_PROT_PIN)
#define DHT_RESET gpio_bit_reset(DHT_GPIO_PROT_PIN)
#define DHT_READ gpio_input_bit_get(DHT_GPIO_PROT_PIN)
#define DHT_OUT gpio_mode_set(DHT_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, DHT_GPIO_PIN)
#define DHT_IN gpio_mode_set(DHT_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DHT_GPIO_PIN)

#define WAIT_LEVEL_CHANGE(level, count, min, max, err_msg) \
    count = 0;                                             \
    do {                                                   \
        delay_1us(1);                                        \
        count++;                                           \
    } while (DHT_READ == level);                            \
    if (count < min || count > max) {                      \
        LOG_DEBUG(err_msg);        \
        return -1;                                         \
    }

void Int_DHT11_Init() {
    rcu_periph_clock_enable(DHT_RCU_GPIO);
    gpio_output_options_set(DHT_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, DHT_GPIO_PIN);
}


char Int_DHT11_GetData(u8 *dat) {
    u8 i;
    u8 count;
    LOG_DEBUG("Int_DHT11_GetData invoke")
    // pull down to awake slave
    DHT_OUT;
    DHT_RESET;
    delay_1ms(19);
    DHT_SET;
    DHT_IN;

    WAIT_LEVEL_CHANGE(SET, count, 10, 35, "on host release");

    WAIT_LEVEL_CHANGE(RESET, count, 78, 88, "on slave response low");
    WAIT_LEVEL_CHANGE(SET, count, 80, 92, "on slave response high");
    // LOG_DEBUGInt("Int_DHT11_GetData", "count", count);

    for (i = 0; i < 40; i++) {
        WAIT_LEVEL_CHANGE(RESET, count, 45, 58, "on slave prefix low");
        // LOG_DEBUGInt("Int_DHT11_GetData", "count", count);

        WAIT_LEVEL_CHANGE(SET, count, 23, 74, "on slave data high");
        dat[i / 8] <<= 1;
        if (count > 27) {
            dat[i / 8] |= 1;
        }
    }

    LOG_DUMP("Int_DHT11_GetData", dat, 5);

    if (dat[0] + dat[1] + dat[2] + dat[3] != dat[4]) {
        LOG_DEBUG("Int_DHT11_GetData checksum error");
        return -1;
    }
    return 0;
}

void Int_DHT11_Convert(u8 *dat, Struct_DHT_Data *st_dat) {
    float humidity;
    float temperature;

    humidity = dat[0] + dat[1] * 0.1f;
    temperature = dat[2] + (dat[3] & 0x7F) * 0.1f;
    if (dat[3] & 0x80) {
        temperature *= -1;
    }

    LOG_DEBUG("Int_DHT11_Convert humidity = %.2f", humidity);
    LOG_DEBUG("Int_DHT11_Convert temperature = %.2f", temperature);

    st_dat->humidity = humidity;
    st_dat->temperature = temperature;
}
