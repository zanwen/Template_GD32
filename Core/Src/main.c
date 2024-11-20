#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "hal_timer.h"
#include "int_rtc8563.h"
#include "int_i2c_oled.h"

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

    I2C_OLED_Init();
    LOG_DEBUG("I2C_OLED_Init done");

    u8 t=' ';
    while (1) {
        I2C_OLED_Refresh();
        delay_1ms(500);
        I2C_OLED_Clear();
        I2C_OLED_ShowChinese(0,0,0,16,1);//中
        I2C_OLED_ShowChinese(18,0,1,16,1);//景
        I2C_OLED_ShowChinese(36,0,2,16,1);//园
        I2C_OLED_ShowChinese(54,0,3,16,1);//电
        I2C_OLED_ShowChinese(72,0,4,16,1);//子
        I2C_OLED_ShowChinese(90,0,5,16,1);//技
        I2C_OLED_ShowChinese(108,0,6,16,1);//术
        I2C_OLED_ShowString(8,16,(u8 *)"ZHONGJINGYUAN",16,1);
        I2C_OLED_ShowString(20,32,(u8 *)"2014/05/01",16,1);
        I2C_OLED_ShowString(0,48,(u8 *)"ASCII:",16,1);
        I2C_OLED_ShowString(63,48,(u8 *)"CODE:",16,1);
        I2C_OLED_ShowChar(48,48,t,16,1);//显示ASCII字符
        t++;
        if(t>'~')t=' ';
        I2C_OLED_ShowNum(103,48,t,3,16,1);
        I2C_OLED_Refresh();
        delay_1ms(500);
        I2C_OLED_Clear();
        I2C_OLED_ShowChinese(0,0,0,16,1);  //16*16 中
        I2C_OLED_ShowChinese(16,0,0,24,1); //24*24 中
        I2C_OLED_ShowChinese(24,20,0,32,1);//32*32 中
        I2C_OLED_ShowChinese(64,0,0,64,1); //64*64 中
        I2C_OLED_Refresh();
        delay_1ms(500);
        I2C_OLED_Clear();
        I2C_OLED_ShowString(0,0,(u8 *)"ABC",8,1);//6*8 “ABC”
        I2C_OLED_ShowString(0,8,(u8 *)"ABC",12,1);//6*12 “ABC”
        I2C_OLED_ShowString(0,20,(u8 *)"ABC",16,1);//8*16 “ABC”
        I2C_OLED_ShowString(0,36,(u8 *)"ABC",24,1);//12*24 “ABC”
        I2C_OLED_Refresh();
        delay_1ms(500);
        I2C_OLED_ScrollDisplay(11,4,1);
    }
}

void on_read_complete(void) {

}