#ifndef __ADC_H
#define __ADC_H

#include "ti_msp_dl_config.h"

#define FFT_LENGTH 1024
#define ADC_LENGTH 1024
#define TRIG_LENGTH 20 //触发点数组长度
#define LOADVALUE_LENGTH 7 //自动重装值数组长度
#define ADC_Trigger 1000 //触发电平

void ADC_Init(void);
uint32_t Calculate_Freq(void);
void Find_TriggerPoint(uint16_t start_index);
void Adjust_LoadValue(void);
uint32_t Calculate_Final(void);
uint16_t Result_Calculate(uint16_t *IndexDiff, uint16_t len);
int compare(const void *a, const void *b);

#endif
