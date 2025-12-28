#ifndef __FFT_H
#define __FFT_H

#include "ti_msp_dl_config.h"

#define MAX_HARMONICS 10 //最大分析谐波数

//谐波分析结构体
typedef struct {
    uint8_t harmonic_num;
    float frequency;
    float magnitude;
    float phase_deg; //原始相位（准确但起初有180度相位模糊）
    float phase_interp_deg; //插值后的相位（无相位模糊）
} HarmonicsInfo;

//窗函数类型结构体
typedef enum {
    WINDOW_RECTANGULAR, //矩形窗
    WINDOW_HANNING, //汉宁窗
    WINDOW_HAMMING, //海明窗
    WINDOW_FLATTOP, //平顶窗
    WINDOW_BLACKMAN //布莱克曼窗
} WindowType;

//复数结构体
typedef struct {
    float real; //实部
    float imag; //虚部
} ComplexFloat;

void DoFFT(void);
void ComputeFFT(uint16_t* signal);
// float Find_FundamentalFreq(void);
void Find_Harmonics(uint32_t Fund_Freq, float fs);
void DisplayResults(void);
float ApplyWindow(float* signal, uint16_t length, WindowType type);
float ApplyRectangularWindow(float* signal, uint16_t length);
float ApplyHanningWindow(float* signal, uint16_t length);
float ApplyHammingWindow(float* signal, uint16_t length);
float ApplyFlattopWindow(float* signal, uint16_t length);
float ApplyBlackmanWindow(float* signal, uint16_t length);
ComplexFloat InterpolateComplexQuadratic(ComplexFloat* values, uint16_t* bins, float fs, uint16_t nfft);
float WrapTo180(float phase_deg);
void InterpolatePhase(HarmonicsInfo* harmonics, uint16_t num);


#endif
