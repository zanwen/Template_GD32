#include "hal_usart0.h"
#include "logger.h"
#include "retarget.h"
#include "systick.h"
#include "int_spi_oled.h"
#include "int_spi_oled_bmp.h"

void on_read_complete(void);

int main(void) {
    NVIC_SetPriorityGrouping(NVIC_PRIGROUP_PRE2_SUB2);
    systick_config();
    retarget_init(USART0);
    hal_usart0_init();
    hal_usart0_read_complete_callabck(on_read_complete);

    SPI_OLED_Init();
    LOG_DEBUG("SPI_OLED_Init");
    SPI_OLED_ColorTurn(0);//0正常显示，1 反色显示
    SPI_OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示

    while (1) {
        SPI_OLED_Display_128x64(bmp1);
        delay_1ms(500);
        SPI_OLED_Display_GB2312_string(0,0,"12864,带中文字库");	/*在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字*/
        SPI_OLED_Display_GB2312_string(0,2,"16X16简体汉字库,");  /*显示一串16x16点阵汉字或8x16的ASCII字.以下雷同*/
        SPI_OLED_Display_GB2312_string(0,4,"或8X16点阵ASCII,");
        SPI_OLED_Display_GB2312_string(0,6,"或5X7点阵ASCII码");
        delay_1ms(500);				
        SPI_OLED_Clear();
        SPI_OLED_Display_GB2312_string(24,0,"中景园电子");	
        SPI_OLED_Display_GB2312_string(0,2,"主要生产OLED模块");
        SPI_OLED_Display_GB2312_string(0,4,"顾客至上真诚服务");
        SPI_OLED_Display_GB2312_string(0,6,"诚信与质量第一！");
        delay_1ms(500);	
        SPI_OLED_Clear();		
        SPI_OLED_Display_GB2312_string(0,0,"GB2312简体字库及");	
        SPI_OLED_Display_GB2312_string(0,2,"有图型功能，可自");	
        SPI_OLED_Display_GB2312_string(0,4,"编大字或图像或生");
        SPI_OLED_Display_GB2312_string(0,6,"僻字，例如：");
        SPI_OLED_Display_16x16(97,6,jiong1);					/*在第7页，第81列显示单个自编生僻汉字“囧”*/
        SPI_OLED_Display_16x16(113,6,lei1);
        delay_1ms(500);
        SPI_OLED_Clear();
        SPI_OLED_Display_GB2312_string(0,0,"<!@#$%^&*()_-+]/");	/*在第1页，第1列，显示一串16x16点阵汉字或8*16的ASCII字*/
        SPI_OLED_Display_string_5x7(0,2,"<!@#$%^&*()_-+]/;.,?[");/*在第3页，第1列，显示一串5x7点阵的ASCII字*/
        SPI_OLED_Display_string_5x7(0,3,"XY electronics Co., ");/*显示一串5x7点阵的ASCII字*/
        SPI_OLED_Display_string_5x7(0,4,"Ltd. established at  ");/*显示一串5x7点阵的ASCII字*/	
        SPI_OLED_Display_string_5x7(0,5,"year 2010.Focus OLED ");/*显示一串5x7点阵的ASCII字*/
        SPI_OLED_Display_string_5x7(0,6,"Mobile:13265585975");/*显示一串5x7点阵的ASCII字*/
        SPI_OLED_Display_string_5x7(0,7,"Tel:0755-32910715");/*显示一串5x7点阵的ASCII字*/
        delay_1ms(500);
        SPI_OLED_Clear();	
        SPI_OLED_Display_GB2312_string(0,0,"啊阿埃挨哎唉哀皑");	/*在第1页，第1列，显示一串16x16点阵汉字或8x16的ASCII字*/
        SPI_OLED_Display_GB2312_string(0,2,"癌蔼矮艾碍爱隘鞍");  /*显示一串16x16点阵汉字或8x16的ASCII字.以下雷同*/
        SPI_OLED_Display_GB2312_string(0,4,"氨安俺按暗岸胺案");
        SPI_OLED_Display_GB2312_string(0,6,"肮昂盎凹敖熬翱袄");		        
        delay_1ms(500);
        SPI_OLED_Clear();	
        SPI_OLED_Display_GB2312_string(0,0,"鬟鬣麽麾縻麂麇麈");
        SPI_OLED_Display_GB2312_string(0,2,"麋麒鏖麝麟黛黜黝");
        SPI_OLED_Display_GB2312_string(0,4,"黠黟黢黩黧黥黪黯");
        SPI_OLED_Display_GB2312_string(0,6,"齄鼬鼹鼷鼽鼾鼢鼯");
        delay_1ms(500);
        SPI_OLED_Clear();
    }
}

void on_read_complete(void) {

}