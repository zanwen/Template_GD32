#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "int_rtc8563.h"
#include "hal_adc.h"

void on_read_complete(void);

 void init_clock() {
     Clock clock;
     clock.year = 2024;
     clock.month = 4;
     clock.day = 19;
     clock.weekday = 6;
     clock.hour = 23;
     clock.minute = 59;
     clock.second = 56;
     Int_RTC_SetClock(&clock);
 }

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);
    
    hal_adc_init();
    
    while (1) {
//        float temp = hal_adc_get_internal_temp();
//        LOG_DEBUG("temperature: %.2f°C", temp);
        float v_potentiometer = hal_adc_get_potentiometer_vol();
        LOG_DEBUG("v_potentiometer = %.1f", v_potentiometer);
        delay_1ms(1000);
    }
}

void on_read_complete(void) {

}