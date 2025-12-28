#include "key.h"

#include "delay.h"
#include "lcd.h"

extern volatile bool ADC0_BusyFlag; //判断ADC0是否忙，0为空闲
extern volatile bool ADC1_BusyFlag; //判断ADC1是否忙，0为空闲
extern volatile bool TimerADC_BusyFlag; //判断定时器是否工作，0为空闲
extern volatile bool TimerStart_Flag; //判断是否要开启定时器，1为开启

//按键初始化
void Key_Init(void)
{
    NVIC_ClearPendingIRQ(KEY_INT_IRQN); //清除中断标志位
    NVIC_EnableIRQ(KEY_INT_IRQN); //使能中断
}

//GPIO的中断服务函数
void GROUP1_IRQHandler(void)
{
    uint32_t GPIO_IIDX = DL_Interrupt_getPendingGroup(DL_INTERRUPT_GROUP_1); //获取中断标志位
    switch (GPIO_IIDX)
    {
        case KEY_INT_IIDX: //若为GPIOB
            if (DL_GPIO_readPins(KEY_PORT, KEY_PIN_B04_PIN) == 0) //PB04
            {
                delay_ms(20);
                if (ADC0_BusyFlag == 0 && ADC1_BusyFlag == 0)
                    TimerStart_Flag = 1;

                while (DL_GPIO_readPins(KEY_PORT, KEY_PIN_B04_PIN) == 0); //等待变为高电平
                delay_ms(20);
            }
            break;

        default:
            break;
    }
}
