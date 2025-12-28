#ifndef __BSP_UART_H
#define __BSP_UART_H

#include "ti_msp_dl_config.h"

#include "string.h"
#include "stdio.h"
#include "stdlib.h"

#define UART_REC_LEN 256 //最大可接收字节数
#define RXBUFFERSIZE 1 //缓存大小

int fputc(int c, FILE* stream); //发送字符
int fputs(const char* restrict s, FILE* restrict stream); //发送字符串
int puts(const char *_ptr); //发送字符串并自动换行
void UART_Init(void);
void UART_Callback(uint8_t rxData);

#endif
