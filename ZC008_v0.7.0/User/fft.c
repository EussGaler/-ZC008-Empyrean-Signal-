#include "fft.h"

#include "stdio.h"
#include "stdlib.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#include <math.h>
#include "adc.h"

extern volatile uint16_t ADC_Value0[ADC_LENGTH]; //ADC0转换后的数据
extern volatile uint16_t ADC_Value1[ADC_LENGTH]; //ADC1转换后的数据
extern volatile uint16_t LoadValue; //定时器的自动重装值
extern WindowType Apply_WindowType; //选择窗类型
extern volatile uint32_t Freq; //由ADC数据计算得到的频率

float FFT_InputBuf[FFT_LENGTH * 2]; //FFT输入数组
float FFT_OutputBuf[FFT_LENGTH]; //FFT输出数组
float Magnitude[FFT_LENGTH/2 + 1]; //单边谱幅值
float Phase[FFT_LENGTH/2 + 1]; //单边谱相位

HarmonicsInfo Harmonics[MAX_HARMONICS]; //储存谐波数据
uint16_t HarmonicsNum = 0; //找到的谐波数

/**
 *  @brief 进行FFT计算分析与结果显示
 */
void DoFFT(void)
{
    float Sample_Rate = 80000000.0f / (1 + LoadValue); //采样频率

    ComputeFFT((uint16_t *)ADC_Value1); //执行FFT分析
    Find_Harmonics(Freq, Sample_Rate); //谐波分析
}

/**
 * @brief 应用窗函数到时域信号（时域加窗）
 *
 * @param[in] signal 信号数组（仅实部，长度为FFT_LENGTH）
 * @param[in] length 信号长度
 * @param[in] type 窗类型
 *
 * @return 窗数组各项之和，用于归一化
 */
float ApplyWindow(float* signal, uint16_t length, WindowType type)
{
    float window_sum = 0;
    switch(type)
    {
        case WINDOW_RECTANGULAR:
            window_sum = ApplyRectangularWindow(signal, length);
            break;
        case WINDOW_HANNING:
            window_sum = ApplyHanningWindow(signal, length);
            break;
        case WINDOW_HAMMING:
            window_sum = ApplyHammingWindow(signal, length);
            break;
        case WINDOW_FLATTOP:
            window_sum = ApplyFlattopWindow(signal, length);
            break;
        case WINDOW_BLACKMAN:
            window_sum = ApplyBlackmanWindow(signal, length);
            break;
        default:
            break;
    }
    return window_sum;
}

//矩形窗
float ApplyRectangularWindow(float* signal, uint16_t length)
{
    return (float)ADC_LENGTH;
}

//汉宁窗
float ApplyHanningWindow(float* signal, uint16_t length)
{
    float window_sum = 0; //用于归一化
    for (uint16_t i = 0; i < length; i++)
    {
        float window = 0.5f * (1.0f - arm_cos_f32(2 * PI * i / (length - 1)));
        signal[i] *= window;
        window_sum += window;
    }
    return window_sum;
}

//海明窗
float ApplyHammingWindow(float* signal, uint16_t length)
{
    float window_sum = 0; //用于归一化
    for (uint16_t i = 0; i < length; i++)
    {
        float window = 0.54f - 0.46f * arm_cos_f32(2 * PI * i / (length - 1));
        signal[i] *= window;
        window_sum += window;
    }
    return window_sum;
}

//平顶窗
float ApplyFlattopWindow(float* signal, uint16_t length)
{
    const float a0 = 0.21557895f;
    const float a1 = 0.41663158f;
    const float a2 = 0.277263158f;
    const float a3 = 0.083578947f;
    const float a4 = 0.006947368f;
    float window_sum = 0; //用于归一化

    for (uint16_t i = 0; i < length; i++)
    {
        float theta = 2 * PI * i / (length - 1);
        float window = a0 
                      - a1 * arm_cos_f32(theta)
                      + a2 * arm_cos_f32(2 * theta)
                      - a3 * arm_cos_f32(3 * theta)
                      + a4 * arm_cos_f32(4 * theta);
        signal[i] *= window;
        window_sum += window;
    }
    return window_sum;
}

//布莱克曼窗
float ApplyBlackmanWindow(float* signal, uint16_t length)
{
    float window_sum = 0; //用于归一化
    for (uint16_t i = 0; i < length; i++)
    {
        float theta = 2 * PI * i / (length - 1);
        float window = 0.42f 
                      - 0.5f * arm_cos_f32(theta)
                      + 0.08f * arm_cos_f32(2 * theta);
        signal[i] *= window;
        window_sum += window;
    }
    return window_sum;
}

/**
 *  @brief 使用对数幅度二次插值法进行复数插值
 *
 *  @param[in] values 相邻3个复数值
 *  @param[in] bins 这三个复数值的下标数组
 *  @param[in] fs 采样频率
 *  @param[in] nfft FFT点数
 *
 *  @return 插值完成后的该点复数值
 */
ComplexFloat InterpolateComplexQuadratic(ComplexFloat* values, uint16_t* bins, float fs, uint16_t nfft)
{
    ComplexFloat result = {0};
    // 1. 计算复数对数
    float log_mag[3], phase[3];
    for (int i = 0; i < 3; i++) {
        log_mag[i] = logf(sqrtf(values[i].real * values[i].real + values[i].imag * values[i].imag) + 1e-12f);
        phase[i] = atan2f(values[i].imag, values[i].real);
    }
    // 2. 相位解卷绕
    if (phase[1] - phase[0] > PI) phase[0] += 2 * PI;
    if (phase[2] - phase[1] > PI) phase[2] -= 2 * PI;
    // 3. 拟合二次多项式
    float k0 = bins[0], k1 = bins[1], k2 = bins[2];
    float a = (log_mag[2] - 2 * log_mag[1] + log_mag[0]) / ((k2 - k1) * (k0 - k1));
    float b = (log_mag[2] - log_mag[0]) / (k2 - k0) - a * (k0 + k2);
    float k_peak = -b / (2 * a);
    // 4. 插值幅值
    float log_mag_peak = a * k_peak * k_peak + b * k_peak + log_mag[1] - a * k1 * k1 - b * k1;
    float mag_peak = expf(log_mag_peak);
    // 5. 插值相位
    float phase_peak = phase[0] + (k_peak - k0) * (phase[2] - phase[0]) / (k2 - k0);
    // 6. 频率
    result.real = k_peak * fs / nfft;
    result.imag = phase_peak;
    return result;

    // ComplexFloat result = {0};
    // if (bins[1] < 1 || bins[1] >= (nfft/2 - 1))
    //     return result; //边界情况不处理

    // //计算三个Bin的对数幅度
    // float log_mag[3];
    // for (int i = 0; i < 3; i++)
    // {
    //     float mag = sqrtf(values[i].real * values[i].real + values[i].imag * values[i].imag);
    //     log_mag[i] = logf(mag + 1e-6f); //防止log(0)
    // }

    // //二次多项式拟合
    // //模型: log_mag = a * k ^ 2 + b * k + c
    // float k0 = (float)bins[0], k1 = (float)bins[1], k2 = (float)bins[2];
    // float a = (log_mag[2] - 2 * log_mag[1] + log_mag[0]) / ((k2 - k1) * (k0 - k1));
    // float b = (log_mag[2] - log_mag[0]) / (k2 - k0) - a * (k0 + k2);
    // float k_peak = - b / (2 * a); //顶点位置

    // //计算插值频率
    // result.real = k_peak * fs / nfft;

    // //计算插值复数值（相位解卷绕）
    // //提取峰值附近的相位
    // float phase_prev = atan2f(values[0].imag, values[0].real);
    // float phase_curr = atan2f(values[1].imag, values[1].real);
    // float phase_next = atan2f(values[2].imag, values[2].real);
    
    // //相位解卷绕
    // if (phase_curr - phase_prev > PI) phase_prev += 2 * PI;
    // if (phase_next - phase_curr > PI) phase_next -= 2 * PI;
    
    // //二次插值相位
    // float phase_interp = phase_prev + (k_peak - k0) * (phase_next - phase_prev) / (k2 - k0);
    // result.imag = phase_interp; //临时存储相位

    // return result;
}

/**
 *  @brief 去除相位模糊得到精确相位
 *
 *  @param[in] harmonics 谐波数组
 *  @param[in] num 谐波数
 */
void InterpolatePhase(HarmonicsInfo* harmonics, uint16_t num)
{
    if (num < 1) return;

    //基波相位
    float base_phase = harmonics[0].phase_deg;
    float base_interp_phase = harmonics[0].phase_interp_deg;

    for (uint16_t i = 1; i < num; i++)
    {
        //计算相位差：当前谐波相位 - i倍基波相位 - (i-1)倍固定偏移90度
        //原始相位差（相对于基波）
        float raw_phase_diff = harmonics[i].phase_deg - i * base_phase - (i - 1) * 90;
        raw_phase_diff = WrapTo180(raw_phase_diff);
        //插值后的相位差（相对于基波）
        float interp_phase_diff = harmonics[i].phase_interp_deg - i * base_interp_phase - (i - 1) * 90;
        interp_phase_diff = WrapTo180(interp_phase_diff);

        //生成两个候选相位差（原始和 +180°）
        float candidate1 = WrapTo180(raw_phase_diff);
        float candidate2 = WrapTo180(raw_phase_diff + 180.0f);

        //比较候选相位差与插值结果的接近程度
        float delta1 = fabsf(WrapTo180(candidate1 - interp_phase_diff));
        float delta2 = fabsf(WrapTo180(candidate2 - interp_phase_diff));

        //选择更接近插值结果的相位差
        harmonics[i].phase_deg = (delta1 <= delta2) ? candidate1 : candidate2;
    }
    harmonics[0].phase_deg = 0; //基波相位差置0
}

/**
 *  @brief 将相位规整至 -180° ~ 180°
 *
 *  @param[in] phase_deg 输入相位
 *
 *  @return 返回规整后的相位
 */
float WrapTo180(float phase_deg)
{
    phase_deg = fmodf(phase_deg + 180.0f, 360.0f);
    if (phase_deg < 0) phase_deg += 360.0f;
    return phase_deg - 180.0f;
}

/**
 *  @brief 显示结果的函数
 */
void DisplayResults(void)
{
    printf("n0.val=%d\xff\xff\xff", Freq);
    printf("x12.val=%d\xff\xff\xff", (int)(100 * Harmonics[1].frequency));

    //通过串口输出
    for (uint16_t i = 0; i < HarmonicsNum; i++)
    {
        if (i > 5) break; //串口屏最大显示范围
        printf("x%d.val=%d\xff\xff\xff", i, (int)(100 * Harmonics[i].magnitude));
        printf("x%d.val=%d\xff\xff\xff", (6 + i), (int)(100 * Harmonics[i].phase_interp_deg));
    }
}

/**
 *  @brief 进行FFT计算
 *
 *  @param[in] signal 输入信号
 *
 */
void ComputeFFT(uint16_t* signal)
{
    //提取原始ADC数据并转换为电压
    float WindowData[FFT_LENGTH]; //临时存储加窗数据
    for (uint16_t i = 0; i < FFT_LENGTH; i++)
    {
        WindowData[i] = signal[i] * 3.3f / 4095.0f;
    }

    //应用窗函数
    float window_sum = ApplyWindow(WindowData, FFT_LENGTH, Apply_WindowType);
    for(uint16_t i = 0; i < FFT_LENGTH; i++)
    {
        FFT_InputBuf[2 * i] = WindowData[i]; //实部
        FFT_InputBuf[2 * i + 1] = 0; //虚部
    }
    
    //执行FFT
    arm_cfft_f32(&arm_cfft_sR_f32_len1024, FFT_InputBuf, 0, 1); //执行FFT，正向且启用位反转，结果覆盖FFT_InputBuf
    
    //计算幅度谱
    arm_cmplx_mag_f32(FFT_InputBuf, FFT_OutputBuf, FFT_LENGTH);
    
    //转换为单边谱
    for (uint16_t i = 0; i <= FFT_LENGTH / 2; i++)
    {
        Magnitude[i] = FFT_OutputBuf[i] / window_sum; //归一化，消除窗函数的影响，对应到真实电压
        if (i > 0 && i < FFT_LENGTH / 2) Magnitude[i] *= 2.0f; //除直流点和奈奎斯特点均进行幅值修正

        //计算相位
        float real = FFT_InputBuf[2 * i];
        float imag = FFT_InputBuf[2 * i + 1];
        Phase[i] = atan2f(imag, real) * 180.0f / PI;
    }
}

/**
 *  @brief 查找谐波分量
 *
 *  @param[in] Fund_Freq 基频
 *  @param[in] fs 采样频率
 */
void Find_Harmonics(uint32_t Fund_Freq, float fs)
{
    float nyquist = fs / 2.0f; //计算奈奎斯特频率，即最高能够分析的频率

    //计算最大可能的谐波次数
    uint16_t maxHarmonics = (uint16_t)(nyquist / Fund_Freq);
    maxHarmonics = (maxHarmonics > MAX_HARMONICS) ? MAX_HARMONICS : maxHarmonics;
    
    HarmonicsNum = 0;
    
    //遍历基波到最高次谐波
    for (uint16_t m = 1; m <= maxHarmonics; m++)
    {
        uint32_t targetFreq = m * Fund_Freq;
        uint16_t binIndex = (uint16_t)roundf(targetFreq * FFT_LENGTH / fs); //由DFT原理得到
        
        if (binIndex >= 1 && binIndex <= (FFT_LENGTH / 2 - 1))
        {
            //提取相邻三个Bin的复数值
            ComplexFloat values[3];
            uint16_t bins[3] = {binIndex-1, binIndex, binIndex+1};
            for (int i = 0; i < 3; i++)
            {
                values[i].real = FFT_InputBuf[2 * bins[i]]; //实部
                values[i].imag = FFT_InputBuf[2 * bins[i] + 1]; //虚部
            }

            //执行插值
            ComplexFloat interp = InterpolateComplexQuadratic(values, bins, fs, FFT_LENGTH);
            
            //存储结果
            Harmonics[HarmonicsNum].harmonic_num = m; //第几次谐波
            Harmonics[HarmonicsNum].frequency = interp.real; //实部即频率
            Harmonics[HarmonicsNum].magnitude = Magnitude[binIndex]; //幅值
            // Harmonics[HarmonicsNum].magnitude = sqrtf(interp.real * interp.real + interp.imag * interp.imag); //插值幅值
            
            //记录原始相位和插值相位
            Harmonics[HarmonicsNum].phase_deg = atan2f(FFT_InputBuf[2 * binIndex + 1], FFT_InputBuf[2 * binIndex]) * 180.0f / PI;
            Harmonics[HarmonicsNum].phase_interp_deg = atan2f(interp.imag, interp.real) * 180.0f / PI;
            
            HarmonicsNum++;
            if (HarmonicsNum >= MAX_HARMONICS) break;
        }
    }
    // InterpolatePhase(Harmonics, HarmonicsNum); //去除相位模糊得到最精确的相位
}
