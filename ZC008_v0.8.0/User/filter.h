#ifndef __FILTER_H
#define __FILTER_H

#include "ti_msp_dl_config.h"

#define SMOOTH_WINDOW 5 //均值滤波窗口大小
#define MEDIAN_WINDOW 5 //中值滤波窗口大小

void Smooth_Filter(uint16_t *src, uint16_t *dst, uint16_t len);
void Median_Filter(uint16_t *src, uint16_t *dst, uint16_t len);

#endif
