#include "adc.h"

#include <stdlib.h>
#include <string.h>

extern volatile uint16_t LoadValue_Set[LOADVALUE_LENGTH]; //预设好的自动重装值
extern volatile uint32_t Freq; //由ADC数据计算得到的频率
extern volatile uint16_t ADC_Value0[ADC_LENGTH]; //ADC0转换后的数据
extern volatile uint16_t ADC_Value1[ADC_LENGTH]; //ADC1转换后的数据
extern volatile uint16_t LoadValue; //定时器的自动重装值
extern volatile uint16_t LoadValue_Index; //自动重装值数组索引
extern volatile bool ADC_BusyFlag; //判断ADC是否忙，0为空闲
extern volatile bool TimerADC_BusyFlag; //判断定时器是否工作，0为空闲
extern volatile bool Freq_FoundFlag; //判断是否已经算出频率，1为算出

bool TriggerFlag = 0; //是否触发，1为找到触发点
uint16_t TriggerIndex[TRIG_LENGTH]; //触发点索引
uint16_t IndexDiff[TRIG_LENGTH]; //触发点索引的逐差
uint16_t TriggerCount = 0; //触发次数
uint16_t IndexDiff_Final = 0; //索引差，用于算频率
uint16_t StartIndex = 0; //从哪里开始找触发点

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
    LoadValue = LoadValue_Set[0]; //初始化自动重装值
    DL_Timer_stopCounter(TIMER_ADC0_INST); //暂停计时即停止ADC转换
    DL_Timer_setLoadValue(TIMER_ADC0_INST, LoadValue); //更改自动重装值

    NVIC_ClearPendingIRQ(ADC_0_INST_INT_IRQN); //清除DMA中断标志
    NVIC_EnableIRQ(ADC_0_INST_INT_IRQN); //使能DMA中断
    NVIC_ClearPendingIRQ(ADC_1_INST_INT_IRQN); //清除DMA中断标志
    NVIC_EnableIRQ(ADC_1_INST_INT_IRQN); //使能DMA中断
}

/**
 * @brief 处理ADC数据算出准确频率
 *
 * @return 得到的频率
 */
uint32_t Calculate_Freq(void)
{
    IndexDiff_Final = 0;
    TriggerCount = 0;
    TriggerFlag = 0;
    StartIndex = 0;

    //找到所有的触发点
    do {
        Find_TriggerPoint(StartIndex);
        if (TriggerFlag == 1)
            StartIndex = TriggerIndex[TriggerCount - 1] + 3;
    }while (TriggerFlag == 1);

    //如果采样到的周期多于10个或者自动重装值不能再调了
    if (TriggerCount >= 10)
    {
        Freq_FoundFlag = 1;
        return Calculate_Final(); //进行最终数据处理
    }
    else if (LoadValue_Index >= (LOADVALUE_LENGTH - 1)) //如果自动重装值不能再调了
    {
        Freq_FoundFlag = 1;
        if (TriggerCount == 0) //没有触发，可能是频率 <= 1Hz 或者幅值低于触发电平
            return 0; //返回频率为0
        else if (TriggerCount == 1) //当频率为1Hz的时候只能触发一次，算出来频率为0
            return 1; //手动返回1
        else
            return Calculate_Final(); //用有限的触发点进行数据处理
    }
    else //采样不足10个
    {
        Adjust_LoadValue(); //调整采样频率

        ADC_BusyFlag = 1;
        TimerADC_BusyFlag = 1;
        DL_TimerG_startCounter(TIMER_ADC0_INST); //开始下一轮的转换
    }
    return 0;
}

/**
 * @brief 查找触发点
 * @param[in] start_index 从哪个索引开始
 */
void Find_TriggerPoint(uint16_t start_index)
{
    //若找到太多点，超出数组范围则直接返回
    if (TriggerCount >= TRIG_LENGTH)
    {
        TriggerFlag = 0;
        return;
    }

    //找触发点
    for (uint16_t i = start_index; i < (ADC_LENGTH - 3); i ++)
    {
        //当前点 <= 触发电平 且 后两点平均值 > 触发电平
        if ((ADC_Value0[i] <= ADC_Trigger) && (ADC_Value0[i+1] > ADC_Trigger))
        {
            TriggerIndex[TriggerCount] = i; //记录触发位置
            TriggerCount ++;
            TriggerFlag = 1;
            return;
        }
    }

    //若一个点都找不到则返回并跳出循环
    TriggerFlag = 0;
}

//自动调整采样频率
void Adjust_LoadValue(void)
{
    if (LoadValue_Index < (LOADVALUE_LENGTH - 1))
    {
        LoadValue_Index ++; //从低到高调整自动重装值
        LoadValue = LoadValue_Set[LoadValue_Index];
        DL_TimerG_setLoadValue(TIMER_ADC0_INST, LoadValue);
    }
}

/**
 * @brief 计算统计结果
 * @param IndexDiff 未排序的原始数组
 * @param len 数组长度
 * @return uint16_t 最终结果（众数或中位数）
 */
uint16_t Result_Calculate(uint16_t *IndexDiff, uint16_t len)
{
    if (len == 0) return 0; //空数组保护

    /*---- 数组排序 ----*/
    uint16_t *sorted = malloc(len * sizeof(uint16_t)); //分配内存
    memcpy(sorted, IndexDiff, len * sizeof(uint16_t)); //复制数组
    qsort(sorted, len, sizeof(uint16_t), compare); //快速排序

    /*---- 计算中位数 ----*/
    float median;
    if (len % 2 == 0)
    {
        median = (sorted[len / 2 - 1] + sorted[len / 2]) / 2.0f;
    }
    else
    {
        median = sorted[len / 2];
    }

    /*---- 寻找众数 ----*/
    uint16_t current_val = sorted[0]; //当前的数
    uint16_t current_cnt = 1; //当前的数出现次数
    uint16_t max_cnt = 1; //众数出现次数
    uint16_t mode = current_val; //众数

    for (uint16_t i = 1; i < len; i++)
    {
        if (sorted[i] == current_val)
        {
            current_cnt ++;
        }
        else
        {
            current_val = sorted[i];
            current_cnt = 1;
        }

        //更新众数（相同次数时取较小值）
        if ((current_cnt > max_cnt) || (current_cnt == max_cnt && current_val < mode))
        {
            max_cnt = current_cnt;
            mode = current_val;
        }
    }

    /*---- 决定最终输出 ----*/
    uint16_t final_result;
    if (max_cnt > 2) //众数出现大于固定次数
    {
        final_result = mode; //有真实众数
    }
    else
    {
        final_result = (uint16_t)(median + 0.5f); //四舍五入取中位数
    }

    free(sorted);
    return final_result;
}

/**
 * @brief 进行最终数据处理
 *
 * @return 得到的频率
 */
uint32_t Calculate_Final(void)
{
    float sample_freq = (CPUCLK_FREQ / (1.0 + LoadValue)); //采样频率

    //计算逐差
    for (uint16_t i = 1; i < TriggerCount; i ++)
    {
        IndexDiff[i - 1] = (TriggerIndex[i] - TriggerIndex[i - 1]);
    }

    //排序并取出中位数
    uint16_t len = TriggerCount - 1;
    IndexDiff_Final = Result_Calculate(IndexDiff, len);

    //计算频率，0.5为四舍五入
    return (uint32_t)((sample_freq / IndexDiff_Final) + 0.5);
}

//比较函数，用于排序
int compare(const void *a, const void *b)
{
    return (*(uint16_t*)a - *(uint16_t*)b);
}

//ADC0的中断服务函数
void ADC0_IRQHandler(void)
{
    switch (DL_ADC12_getPendingInterrupt(ADC_0_INST)) 
    {
        case DL_ADC12_IIDX_DMA_DONE: //若为DMA搬运完成中断
            ADC_BusyFlag = 0; //忙标志置0，防止在中断中处理数据阻塞程序
            break;
        default:
            break;
    }
}
