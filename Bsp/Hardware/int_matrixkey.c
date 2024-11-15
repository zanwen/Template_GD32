#include <stdbool.h>
#include "int_matrixkey.h"
#include "com_util.h"

#define ROWS 4
#define COLS 4

#define KEY_GPIO_PORT       GPIOE
#define KEY_GPIO_RCU        RCU_GPIOE

#define KEY_COL0_PIN        GPIO_PIN_12
#define KEY_COL1_PIN        GPIO_PIN_13
#define KEY_COL2_PIN        GPIO_PIN_14
#define KEY_COL3_PIN        GPIO_PIN_15
#define KEY_ALL_COLS_PIN    (KEY_COL0_PIN | KEY_COL1_PIN | KEY_COL2_PIN | KEY_COL3_PIN)

#define KEY_ROW0_PIN        GPIO_PIN_8
#define KEY_ROW1_PIN        GPIO_PIN_9
#define KEY_ROW2_PIN        GPIO_PIN_10
#define KEY_ROW3_PIN        GPIO_PIN_11
#define KEY_ALL_ROWS_PIN    (KEY_ROW0_PIN | KEY_ROW1_PIN | KEY_ROW2_PIN | KEY_ROW3_PIN)

static uint16_t s_key_states = 0xFFFF;
// static uint8_t s_key_states[16] = {1};
static MatrixKey_Callback s_key_up_callback = NULL;
static MatrixKey_Callback s_key_down_callback = NULL;

static bool Int_MatrixKey_ColIsUp(uint8_t i);
static bool Int_MatrixKey_ColIsDown(uint8_t i);

void Int_MatrixKey_Init() {
    rcu_periph_clock_enable(KEY_GPIO_RCU);

    // Initialize columns as inputs (PE12~15)
    gpio_mode_set(KEY_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, KEY_ALL_COLS_PIN);

    // Initialize rows as outputs (PE8~11)
    gpio_mode_set(KEY_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, KEY_ALL_ROWS_PIN);
    gpio_output_options_set(KEY_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, KEY_ALL_ROWS_PIN);

    // Set all rows to high initially
    gpio_bit_set(KEY_GPIO_PORT, KEY_ALL_ROWS_PIN);
}

void Int_MatrixKey_SetDownRow(uint8_t i) {
    // Set all rows to high
    gpio_bit_set(KEY_GPIO_PORT, KEY_ALL_ROWS_PIN);

    // Set the selected row to low
    switch (i) {
        case 0:
            gpio_bit_reset(KEY_GPIO_PORT, KEY_ROW0_PIN);
            break;
        case 1:
            gpio_bit_reset(KEY_GPIO_PORT, KEY_ROW1_PIN);
            break;
        case 2:
            gpio_bit_reset(KEY_GPIO_PORT, KEY_ROW2_PIN);
            break;
        case 3:
            gpio_bit_reset(KEY_GPIO_PORT, KEY_ROW3_PIN);
            break;
        default:
            // Invalid row, do nothing
            break;
    }
}

bool Int_MatrixKey_ColIsUp(uint8_t i) {
    return !Int_MatrixKey_ColIsDown(i);
}

bool Int_MatrixKey_ColIsDown(uint8_t i) {
    switch (i) {
        case 0 :
            return gpio_input_bit_get(KEY_GPIO_PORT, KEY_COL0_PIN) == RESET;
        case 1 :
            return gpio_input_bit_get(KEY_GPIO_PORT, KEY_COL1_PIN) == RESET;
        case 2 :
            return gpio_input_bit_get(KEY_GPIO_PORT, KEY_COL2_PIN) == RESET;
        case 3 :
            return gpio_input_bit_get(KEY_GPIO_PORT, KEY_COL3_PIN) == RESET;
        default :
            return false;
    }
}

char Int_MatrixKey_Detect() {
    uint8_t i, j, k;
    char key_no = -1;
    for (i = 0; i < ROWS; i++) {
        Int_MatrixKey_SetDownRow(i);
        for (j = 0; j < COLS; j++) {
            k = i * COLS + j;
            if (Int_MatrixKey_ColIsDown(j) && (s_key_states & (1 << k))) {
                // set 0
                s_key_states &= ~(1 << k);

                key_no = i * COLS + j + 1;

                if (s_key_down_callback) {
                    s_key_down_callback(i, j);
                }
            }
            if (Int_MatrixKey_ColIsUp(j) && (s_key_states & (1 << k)) == 0) {
                // set 1
                s_key_states |= (1 << k);
                if (s_key_up_callback) {
                    s_key_up_callback(i, j);
                }
            }
        }
    }
    return key_no;
}

void Int_MatrixKey_GetPressedKeys(uint8_t* keybuf, uint8_t* sizebuf) {
    uint8_t i, j, k;
    *sizebuf = 0;
    for (i = 0; i < ROWS; i++) {
        Int_MatrixKey_SetDownRow(i);
        for (j = 0; j < COLS; j++) {
            k = i * COLS + j;
            if (Int_MatrixKey_ColIsDown(j) && (s_key_states & (1 << k))) {
                // set 0
                s_key_states &= ~(1 << k);
                // printf("[%d, %d] change to down\n", (int)i, (int)j);
                keybuf[(*sizebuf)++] = i * COLS + j;
            }
            if (Int_MatrixKey_ColIsUp(j) && (s_key_states & (1 << k)) == 0) {
                // set 1
                s_key_states |= (1 << k);
                // printf("[%d, %d] change to up\n", (int)i, (int)j);
            }
        }
    }
}

void Int_MatrixKey_RegisterKeyDownCallabck(MatrixKey_Callback callback) {
    s_key_down_callback = callback;
}

void Int_MatrixKey_RegisterKeyUpCallabck(MatrixKey_Callback callback) {
    s_key_up_callback = callback;
}

/* void Int_MatrixKey_Detect() {
    uint8_t i, j, k;
    for (i = 0; i < ROWS; i++) {
        Int_MatrixKey_SetDownRow(i);
        for (j = 0; j < COLS; j++) {
            k = i * COLS + j;
            if (Int_MatrixKey_ColIsDown(j) && s_key_states[k] == 1) {
                // set 0
                s_key_states[k] = 0;
                printf("%d row %d col is pressed\n", (int)i, (int)j);
            }
            if (Int_MatrixKey_ColIsUp(j) && s_key_states[k] == 0) {
                printf("%d row %d col is released\n", (int)i, (int)j);
                // set 1
                s_key_states[k] = 1;
            }
        }
    }
    Com_Util_Delayms(50);
} */