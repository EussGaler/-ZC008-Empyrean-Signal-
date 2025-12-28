#include "ti/driverlib/dl_gpio.h"
#include "ti/driverlib/m0p/dl_core.h"
#include "ti_msp_dl_config.h"
#include <stdio.h>

#include "delay.h"
#include "lcd_init.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"

volatile uint16_t LoadValue_Set[LOADVALUE_LENGTH] = {39U, 79U, 499U, 799U, 4999U, 7999U, 65519U}; //预设好的自动重装值
volatile uint32_t Freq = 0; //由ADC数据计算得到的频率
volatile uint16_t ADC_Value0[ADC_LENGTH] = {0}; //ADC0转换的数据
volatile uint16_t ADC_Value1[ADC_LENGTH] = {0}; //ADC1转换的数据
volatile uint16_t LoadValue = 0; //定时器的自动重装值
volatile uint16_t LoadValue_Index = 0; //自动重装值数组索引
volatile bool ADC0_BusyFlag = 0; //判断ADC0是否忙，0为空闲
volatile bool ADC1_BusyFlag = 0; //判断ADC1是否忙，0为空闲
volatile bool TimerADC_BusyFlag = 0; //判断定时器是否工作，0为空闲
volatile bool TimerStart_Flag = 0; //判断是否要开启定时器，1为开启
volatile bool Freq_FoundFlag = 0; //判断是否已经算出频率，1为算出
volatile bool FinalSample_Flag = 0; //判断是否进行了最终采样，1为是

int main(void)
{
	SYSCFG_DL_init(); //SYSCFG初始化
    delay_ms(200);
    LCD_Init(); //初始化LCD
    delay_ms(200);
    Key_Init();
    delay_ms(200);
    ADC_Init();

    DL_GPIO_setPins(LED_PORT, LED_PIN_LED_PIN); //测试用LED
    printf("By EussGaler"); //串口输出测试
    LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

    while(1)
    {
        if (TimerStart_Flag == 1)
        {
            ADC0_BusyFlag = 1;
            ADC1_BusyFlag = 1;
            TimerADC_BusyFlag = 1;
            TimerStart_Flag = 0;
            Freq_FoundFlag = 0;

            LoadValue_Index = 0; //初始化自动重装值
            DL_Timer_setLoadValue(TIMER_ADC0_INST, LoadValue_Set[0]); //更改自动重装值
            DL_Timer_startCounter(TIMER_ADC0_INST); //开启定时器

            LCD_Fill(10, 10, LCD_W, 30, WHITE);
            LCD_ShowString(10, 10, (uint8_t *)"Start", GREEN, WHITE, 16, 1);
        }

        if (ADC0_BusyFlag == 0 && ADC1_BusyFlag == 0 && TimerADC_BusyFlag == 1)
        {
            DL_Timer_stopCounter(TIMER_ADC0_INST); //停止转换
            TimerADC_BusyFlag = 0;

            if (Freq_FoundFlag == 0)
                Freq = Calculate_Freq(); //获取频率

            //频率 >= 2 说明至少采到1个周期，此时可以进行最终处理
            if (Freq_FoundFlag == 1 && Freq >= 2)
            {
                if (FinalSample_Flag == 1)
                {
                    __BKPT(0);
                    FinalSample_Flag = 0;
                }
                else
                {
                    LoadValue = (CPUCLK_FREQ / (Freq * 204.8)) - 1; //选取合适的自动重装值以恰好采样5个周期
                    if (LoadValue >= 79U && LoadValue <= 65519U) //在合理范围内
                        DL_Timer_setLoadValue(TIMER_ADC0_INST, LoadValue); //更改自动重装值
                    else
                        __BKPT(0);

                    ADC0_BusyFlag = 1;
                    ADC1_BusyFlag = 1;
                    TimerADC_BusyFlag = 1;
                    FinalSample_Flag = 1;
                    DL_Timer_startCounter(TIMER_ADC0_INST); //开启定时器
                }
            }
        }

        delay_ms(10);
    }
}

//防止进入意外中断Default_Handler
void NMI_Handler(void){ __BKPT(0);} //不可屏蔽中断函数
void HardFault_Handler(void){ __BKPT(0);} //硬件故障中断函数
void SVC_Handler(void){ __BKPT(0);} //特权中断函数
void PendSV_Handler(void){ __BKPT(0);} //一种可挂起的、最低优先级的中断函数
void SysTick_Handler(void){ __BKPT(0);} //滴答定时器中断函数
void GROUP0_IRQHandler(void){ __BKPT(0);} //GROUP0的中断函数
// void GROUP1_IRQHandler(void){ __BKPT(0);} //GROUP1中断函数
void TIMG8_IRQHandler(void){ __BKPT(0);} //TIMG8的中断函数
void UART3_IRQHandler(void){ __BKPT(0);} //UART3的中断函数
// void ADC0_IRQHandler(void){ __BKPT(0);} //ADC0的中断函数
// void ADC1_IRQHandler(void){ __BKPT(0);} //ADC1的中断函数
void CANFD0_IRQHandler(void){ __BKPT(0);} //CANFD0的中断函数
void DAC0_IRQHandler(void){ __BKPT(0);} //DAC0的中断函数
void SPI0_IRQHandler(void){ __BKPT(0);} //SPI0的中断函数
void SPI1_IRQHandler(void){ __BKPT(0);} //SPI1的中断函数
void UART1_IRQHandler(void){ __BKPT(0);} //UART1的中断函数
void UART2_IRQHandler(void){ __BKPT(0);} //UART2的中断函数
void UART0_IRQHandler(void){ __BKPT(0);} //UART0的中断函数
void TIMG0_IRQHandler(void){ __BKPT(0);} //TIMG0的中断函数
void TIMG6_IRQHandler(void){ __BKPT(0);} //TIMG6的中断函数
void TIMA0_IRQHandler(void){ __BKPT(0);} //TIMA0的中断函数
void TIMA1_IRQHandler(void){ __BKPT(0);} //TIMA1的中断函数
void TIMG7_IRQHandler(void){ __BKPT(0);} //TIMG7的中断函数
void TIMG12_IRQHandler(void){ __BKPT(0);} //TIMG12的中断函数
void I2C0_IRQHandler(void){ __BKPT(0);} //I2C0的中断函数
void I2C1_IRQHandler(void){ __BKPT(0);} //I2C1的中断函数
void AES_IRQHandler(void){ __BKPT(0);} //硬件加速器的中断函数
void RTC_IRQHandler(void){ __BKPT(0);} //RTC实时时钟的中断函数
void DMA_IRQHandler(void){ __BKPT(0);} //DMA的中断函数
