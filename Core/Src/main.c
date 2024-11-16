#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "hal_wdgt.h"
#include "int_matrixkey.h"
#include "int_nixietube.h"
#include "int_dht11.h"


void on_read_complete(void);


int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    uint8_t buf[5] = {0};
    Struct_DHT_Data st_dht;
    Int_DHT11_Init();
    Int_DHT11_GetData(buf);
    Int_DHT11_Convert(buf, &st_dht);

    while (1) {
    }
}

void on_read_complete(void) {

}
