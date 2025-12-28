#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

#include "ti_msp_dl_config.h"

void delay_us(unsigned long __us); //搭配滴答定时器实现的精确us延时
void delay_ms(unsigned long ms); //搭配滴答定时器实现的精确ms延时
void delay_1us(unsigned long __us);
void delay_1ms(unsigned long ms);

#endif
