#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include <stdio.h>

#include "delay.h"
#include "lcd_init.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"

volatile uint16_t ADC_Value[ADC_LENGTH] = {0}; //ADC转换的数据
volatile uint16_t LoadValue = 0; //定时器的自动重装值
volatile uint8_t ADC0_InterruptFlag = 1; //判断是否发生ADC0中断，1为发生

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
        
    }
}
