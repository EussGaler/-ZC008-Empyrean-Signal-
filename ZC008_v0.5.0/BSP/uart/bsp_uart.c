#include "bsp_uart.h"

extern unsigned char UART0_RxBuff[UART_REC_LEN]; //接收缓存
extern unsigned int UART0_RxSTA; //接收状态标记
extern unsigned char UART0_len; //接收的数据长度
unsigned char aRxBuffer0[RXBUFFERSIZE]; //接收缓冲

//发送字符
int fputc(int c, FILE* stream)
{
    DL_UART_Main_transmitDataBlocking(UART_0_INST, c);
    return c;
}

//发送字符串
int fputs(const char* restrict s, FILE* restrict stream)
{
    uint16_t i, len;
    len = strlen(s);
    for(i = 0; i < len; i ++)
    {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, s[i]);
    }
    return len;
}

//发送字符串并自动换行
int puts(const char *_ptr)
{
    int count = fputs(_ptr, stdout);
    count += fputs("\n", stdout);
    return count;
}

//初始化UART
void UART_Init(void)
{
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

// UART中断回调函数 (完全重写)
void UART_Callback(uint8_t rxData)
{
    //0x8000: 接收完成标志, 0x4000: 收到\r标志
    if (UART0_RxSTA & 0x8000) return; //已接收完成，忽略新数据
    
    //处理结束符
    if (rxData == 0x0D) //收到\r
    {
        UART0_RxSTA |= 0x4000;
        return;
    }
    else if (rxData == 0x0A) //收到\n
    {
        if (UART0_RxSTA & 0x4000) //前一个是\r
        {
            UART0_RxSTA |= 0x8000;  //标记接收完成
            UART0_len = UART0_RxSTA & 0x3FFF; //记录长度
            return;
        }
    }
    
    //普通数据
    uint16_t idx = UART0_RxSTA & 0x3FFF;
    if (idx < UART_REC_LEN - 1)
    {
        UART0_RxBuff[idx] = rxData;
        UART0_RxSTA = (UART0_RxSTA & 0xC000) | (idx + 1); //更新长度
    }
    else
        UART0_RxSTA = 0; //缓冲区溢出，重置状态
}

//UART0中断服务函数
void UART_0_INST_IRQHandler(void)
{
    uint8_t rxData;

    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST))
    {
        case DL_UART_MAIN_IIDX_RX:
            rxData = DL_UART_Main_receiveData(UART_0_INST);
            UART_Callback(rxData);

            break;
        default:
            break;
    }
}
