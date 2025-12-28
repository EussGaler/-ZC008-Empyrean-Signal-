#include "adc.h"

#include "lcd.h"

extern volatile uint16_t ADC_Value0[FFT_LENGTH]; //ADC0转换后的数据
extern volatile uint16_t ADC_Value1[FFT_LENGTH]; //ADC1转换后的数据
extern volatile uint16_t LoadValue; //定时器的自动重装值
extern volatile bool ADC0_BusyFlag; //判断ADC0是否忙，0为空闲
extern volatile bool ADC1_BusyFlag; //判断ADC1是否忙，0为空闲

static uint16_t LoadValue_Set[LOADVALUE_LENGTH] = {79U, 499U, 799U, 4999U, 7999U, 65519U}; //预设好的自动重装值
uint16_t LoadValue_Index = 0; //自动重装值数组索引

//ADC初始化
void ADC_Init(void)
{
    DL_DMA_setSrcAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ADC0->ULLMEM.MEMRES[0]); //设置DMA搬运的起始地址
    DL_DMA_setDestAddr(DMA, DMA_CH0_CHAN_ID, (uint32_t) &ADC_Value0[0]); //设置DMA搬运的目的地址
    DL_DMA_setTransferSize(DMA, DMA_CH0_CHAN_ID, ADC_LENGTH); //设置DMA搬运长度
    DL_DMA_enableChannel(DMA, DMA_CH0_CHAN_ID); //开启DMA

    DL_DMA_setSrcAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &ADC1->ULLMEM.MEMRES[0]); //设置DMA搬运的起始地址
    DL_DMA_setDestAddr(DMA, DMA_CH1_CHAN_ID, (uint32_t) &ADC_Value1[0]); //设置DMA搬运的目的地址
    DL_DMA_setTransferSize(DMA, DMA_CH1_CHAN_ID, ADC_LENGTH); //设置DMA搬运长度
    DL_DMA_enableChannel(DMA, DMA_CH1_CHAN_ID); //开启DMA
    LoadValue = LoadValue_Set[2]; //初始化自动重装值
    DL_Timer_stopCounter(TIMER_ADC0_INST); //暂停计时即停止ADC转换
    DL_Timer_setLoadValue(TIMER_ADC0_INST, LoadValue); //更改自动重装值

    NVIC_ClearPendingIRQ(ADC_0_INST_INT_IRQN); //清除DMA中断标志
    NVIC_EnableIRQ(ADC_0_INST_INT_IRQN); //使能DMA中断
    NVIC_ClearPendingIRQ(ADC_1_INST_INT_IRQN); //清除DMA中断标志
    NVIC_EnableIRQ(ADC_1_INST_INT_IRQN); //使能DMA中断
}

//ADC0的中断服务函数
void ADC0_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_0_INST)) 
    {
        case DL_ADC12_IIDX_DMA_DONE: //若为DMA搬运完成中断
            ADC0_BusyFlag = 0; //忙标志置0，防止在中断中处理数据阻塞程序
            break;
        default:
            break;
    }
}

//ADC1的中断服务函数
void ADC1_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_1_INST)) 
    {
        case DL_ADC12_IIDX_DMA_DONE: //若为DMA搬运完成中断
            ADC1_BusyFlag = 0; //忙标志置0，防止在中断中处理数据阻塞程序
            break;
        default:
            break;
    }
}
