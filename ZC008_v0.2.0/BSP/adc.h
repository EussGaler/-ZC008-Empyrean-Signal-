#ifndef __ADC_H
#define __ADC_H

#include "ti_msp_dl_config.h"

#define FFT_LENGTH 1030 //1024+6防数组越界
#define ADC_LENGTH 1024
#define LOADVALUE_LENGTH 6

void ADC_Init(void);

#endif
