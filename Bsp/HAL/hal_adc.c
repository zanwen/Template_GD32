//
// Created by 86157 on 2024/11/19.
//

#include "hal_adc.h"
#include "com_typedef.h"
#include "systick.h"

#define ADC_CHANNEL_SIZE 3
#define ADC_SEQ_TEMPERATURE 0
#define ADC_SEQ_POTENTIOMETER 1
#define ADC_SEQ_NTC_RESISTOR 2
#define ADC_DMA_CHANNEL DMA1, DMA_CH0
#define ADC_MAX 4096
#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define ABS(value) ((value) > 0 ? (value) : -(value))
#define ADC_V_REF 3.3
#define NTC_VCC 3.3

static uint16_t adc_value[ADC_CHANNEL_SIZE] = {0};

/*region temp_table*/
static u16 temp_table[] = {
        58354,// -55
        55464,// -54
        52698,// -53
        50048,// -52
        47515,// -51
        45097,// -50
        42789,// -49
        40589,// -48
        38492,// -47
        36496,// -46
        34597,// -45
        32791,// -44
        31075,// -43
        29444,// -42
        27896,// -41
        26427,// -40
        25034,// -39
        23713,// -38
        22460,// -37
        21273,// -36
        20148,// -35
        19083,// -34
        18075,// -33
        17120,// -32
        16216,// -31
        15361,// -30
        14551,// -29
        13785,// -28
        13061,// -27
        12376,// -26
        11728,// -25
        11114,// -24
        10535,// -23
        9986, // -22
        9468, // -21
        8977, // -20
        8513, // -19
        8075, // -18
        7660, // -17
        7267, // -16
        6896, // -15
        6545, // -14
        6212, // -13
        5898, // -12
        5601, // -11
        5319, // -10
        5053, // -9
        4801, // -8
        4562, // -7
        4336, // -6
        4122, // -5
        3920, // -4
        3728, // -3
        3546, // -2
        3374, // -1
        3211, // 0
        3057, // 1
        2910, // 2
        2771, // 3
        2639, // 4
        2515, // 5
        2396, // 6
        2284, // 7
        2177, // 8
        2076, // 9
        1978, // 10
        1889, // 11
        1802, // 12
        1720, // 13
        1642, // 14
        1568, // 15
        1497, // 16
        1430, // 17
        1366, // 18
        1306, // 19
        1248, // 20
        1193, // 21
        1141, // 22
        1092, // 23
        1044, // 24
        1000, // 25
        957,  // 26
        916,  // 27
        877,  // 28
        840,  // 29
        805,  // 30
        771,  // 31
        739,  // 32
        709,  // 33
        679,  // 34
        652,  // 35
        625,  // 36
        600,  // 37
        576,  // 38
        552,  // 39
        530,  // 40
        509,  // 41
        489,  // 42
        470,  // 43
        452,  // 44
        434,  // 45
        417,  // 46
        401,  // 47
        386,  // 48
        371,  // 49
        358,  // 50
        344,  // 51
        331,  // 52
        318,  // 53
        306,  // 54
        295,  // 55
        284,  // 56
        274,  // 57
        264,  // 58
        254,  // 59
        245,  // 60
        236,  // 61
        228,  // 62
        220,  // 63
        212,  // 64
        205,  // 65
        198,  // 66
        191,  // 67
        184,  // 68
        178,  // 69
        172,  // 70
        166,  // 71
        160,  // 72
        155,  // 73
        150,  // 74
        145,  // 75
        140,  // 76
        135,  // 77
        131,  // 78
        126,  // 79
        122,  // 80
        118,  // 81
        115,  // 82
        111,  // 83
        107,  // 84
        104,  // 85
        101,  // 86
        97,   // 87
        94,   // 88
        91,   // 89
        89,   // 90
        86,   // 91
        83,   // 92
        81,   // 93
        78,   // 94
        76,   // 95
        74,   // 96
        71,   // 97
        69,   // 98
        67,   // 99
        65,   // 100
        63,   // 101
        61,   // 102
        60,   // 103
        58,   // 104
        56,   // 105
        55,   // 106
        53,   // 107
        52,   // 108
        50,   // 109
        49,   // 110
        47,   // 111
        46,   // 112
        45,   // 113
        43,   // 114
        42,   // 115
        41,   // 116
        40,   // 117
        39,   // 118
        38,   // 119
        37,   // 120
        36,   // 121
        35,   // 122
        34,   // 123
        33,   // 124
        32,   // 125
};
/*endregion*/

static void dma_config(void) {
    rcu_periph_clock_enable(RCU_DMA1);
    /* ADC_DMA_channel configuration */
    dma_single_data_parameter_struct dma_single_data_parameter;

    /* ADC DMA_channel configuration */
    dma_deinit(ADC_DMA_CHANNEL);

    /* initialize DMA single data mode */
    dma_single_data_parameter.periph_addr = (uint32_t) (&ADC_RDATA(ADC0));
    dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_single_data_parameter.memory0_addr = (uint32_t) (adc_value);
    dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    dma_single_data_parameter.number = ADC_CHANNEL_SIZE;
    dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(ADC_DMA_CHANNEL, &dma_single_data_parameter);
    dma_channel_subperipheral_select(ADC_DMA_CHANNEL, DMA_SUBPERI0);

    /* enable DMA circulation mode */
    dma_circulation_enable(ADC_DMA_CHANNEL);

    /* enable DMA channel */
    dma_channel_enable(ADC_DMA_CHANNEL);
}

static void adc_config(void) {
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);

    adc_deinit();
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC contineous function disable */
    // trigger manually by software
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    /* ADC scan mode disable */
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);

    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, ADC_CHANNEL_SIZE);
    adc_routine_channel_config(ADC0, ADC_SEQ_TEMPERATURE, ADC_CHANNEL_16, ADC_SAMPLETIME_15);
    adc_routine_channel_config(ADC0, ADC_SEQ_POTENTIOMETER, ADC_CHANNEL_14, ADC_SAMPLETIME_15);
    adc_routine_channel_config(ADC0, ADC_SEQ_NTC_RESISTOR, ADC_CHANNEL_1, ADC_SAMPLETIME_15);

    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH, ENABLE);

    /* ADC DMA function enable */
    adc_dma_request_after_last_enable(ADC0);
    adc_dma_mode_enable(ADC0);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
}

static void gpio_config() {
    // PC4 potentiometer, analog input
    rcu_periph_clock_enable(RCU_GPIOC);
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);

    // PA1 NTC Resistor, analog input
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
}

void hal_adc_init(void) {
    gpio_config();
    dma_config();
    adc_config();
}
void hal_adc_trigger(void) {
    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
    while (dma_flag_get(ADC_DMA_CHANNEL, DMA_FLAG_FTF) == RESET)
        ;
    dma_flag_clear(ADC_DMA_CHANNEL, DMA_FLAG_FTF);
}

static float parse_temperature() {
    float v_temp = adc_value[ADC_SEQ_TEMPERATURE] * 3.3 / ADC_MAX;
    return (1.45 - v_temp) * 1000 / 4.1 + 25;
}

static float parse_potentiometer() {
    return adc_value[ADC_SEQ_POTENTIOMETER] * 3.3 / ADC_MAX;
}

static u16 search_index(float value) {
    u16 i;
    u16 len = ARRAY_LEN(temp_table);
    u16 min_index = 0;
    value *= 100;
    for (i = 1; i < len; i++) {
        if (ABS(temp_table[i] - value) < ABS(temp_table[min_index] - value)) {
            min_index = i;
        }
    }
    return min_index;
}

static float parse_ntc_resistor() {
    // adc / 4096 = v / 2.5
    float voltage = adc_value[ADC_SEQ_NTC_RESISTOR] * ADC_V_REF / ADC_MAX;
    // voltage / resistance = (VCC - voltage) / 10k
    float resistance = voltage * 10 / (NTC_VCC - voltage);
    return search_index(resistance) - 55;
}

void hal_adc_measure(adc_result_t *buf) {
    hal_adc_trigger();
    buf->internal_temperature = parse_temperature();
    buf->potentiometer_vol = parse_potentiometer();
    buf->ntc_temperature = parse_ntc_resistor();
}
