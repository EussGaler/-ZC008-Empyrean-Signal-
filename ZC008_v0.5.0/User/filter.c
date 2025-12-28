#include "filter.h"

#include "adc.h"

extern volatile uint16_t ADC_Value0[ADC_LENGTH]; //ADC0转换后的数据
extern volatile uint16_t ADC_Value1[ADC_LENGTH]; //ADC1转换后的数据

//滑动平均滤波
void Smooth_Filter(uint16_t *src, uint16_t *dst, uint16_t len)
{
    for (int i = 0; i < len; i++)
    {
        int sum = 0;
        int count = 0;
        for (int j = -SMOOTH_WINDOW / 2; j <= SMOOTH_WINDOW / 2; j++)
        {
            int idx = i + j;
            if (idx >= 0 && idx < len)
            {
                sum += src[idx];
                count++;
            }
        }
        dst[i] = sum / count;
    }
}

//中值滤波
void Median_Filter(uint16_t *src, uint16_t *dst, uint16_t len)
{
    uint16_t window[MEDIAN_WINDOW];
    for (int i = 0; i < len; i++)
    {
        int count = 0;
        for (int j = -MEDIAN_WINDOW / 2; j <= MEDIAN_WINDOW / 2; j++)
        {
            int idx = i + j;
            if (idx >= 0 && idx < len)
                window[count++] = src[idx];
        }
        //排序，从小到大
        for (int m = 0; m < count - 1; m++)
        {
            for (int n = m+1; n < count; n++)
            {
                if (window[m] > window[n])
                {
                    uint16_t tmp = window[m];
                    window[m] = window[n];
                    window[n] = tmp;
                }
            }
        }
        dst[i] = window[count / 2];
    }
}
