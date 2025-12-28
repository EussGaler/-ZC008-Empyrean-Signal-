#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include <stdio.h>

#include "delay.h"
#include "lcd_init.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"

volatile uint16_t ADC_Value0[ADC_LENGTH] = {0}; //ADC0转换的数据
volatile uint16_t ADC_Value1[ADC_LENGTH] = {0}; //ADC1转换的数据
volatile uint16_t LoadValue = 0; //定时器的自动重装值
volatile bool ADC0_BusyFlag = 0; //判断ADC0是否忙，0为空闲
volatile bool ADC1_BusyFlag = 0; //判断ADC1是否忙，0为空闲
volatile bool TimerADC_BusyFlag = 0; //判断定时器是否工作，0为空闲

int main(void)
{
	SYSCFG_DL_init(); //SYSCFG初始化

    DL_GPIO_setPins(LED_PORT, LED_PIN_LED_PIN); //测试用LED
    printf("By EussGaler"); //串口输出测试

    LCD_Init(); //初始化LCD
    delay_ms(200);
    Key_Init();
    delay_ms(200);
    ADC_Init();

    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

    while(1)
    {
        if (ADC0_BusyFlag == 0 && ADC1_BusyFlag == 0 && TimerADC_BusyFlag == 1)
        {
            DL_Timer_stopCounter(TIMER_ADC0_INST); //停止转换
            TimerADC_BusyFlag = 0;
        }
    }
}
