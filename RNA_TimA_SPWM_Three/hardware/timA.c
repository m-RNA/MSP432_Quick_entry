/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

#include "timA.h"

/**************     SPWM波相关开始     **************/
double MaxDutyCycle = 0.5f;         //最大占空比时，所除比例系数
uint16_t CarrierFrequency = 20000;  //SPWM波载波频率，单位Hz
uint16_t FundamentalFrequency = 50; //SPWM波基波频率，单位HZ

static uint8_t Sector = 1;            //扇区
static uint16_t SineTablePos_1 = 87;  //+120
static uint16_t SineTablePos_2 = 0;   //0
static uint16_t SineTablePos_3 = 175; //-120

/* 正弦表 */
static uint16_t DutyCycleSineWaveTable[SamplesPerCycle] = {
    0x04B7, 0x04D3, 0x04F0, 0x050C, 0x0529, 0x0545, 0x0561, 0x057E, 0x059A, 0x05B6, 0x05D1, 0x05ED, 0x0608, 0x0624, 0x063F, 0x065A,
    0x0674, 0x068E, 0x06A8, 0x06C2, 0x06DC, 0x06F5, 0x070E, 0x0726, 0x073E, 0x0756, 0x076D, 0x0784, 0x079B, 0x07B1, 0x07C7, 0x07DC,
    0x07F1, 0x0805, 0x0819, 0x082C, 0x083F, 0x0851, 0x0863, 0x0874, 0x0885, 0x0895, 0x08A5, 0x08B4, 0x08C2, 0x08D0, 0x08DD, 0x08EA,
    0x08F6, 0x0901, 0x090C, 0x0916, 0x0920, 0x0929, 0x0931, 0x0938, 0x093F, 0x0946, 0x094B, 0x0950, 0x0954, 0x0958, 0x095B, 0x095D,
    0x095E, 0x095F, 0x095F, 0x095F, 0x095E, 0x095C, 0x0959, 0x0956, 0x0952, 0x094E, 0x0948, 0x0943, 0x093C, 0x0935, 0x092D, 0x0924,
    0x091B, 0x0911, 0x0907, 0x08FC, 0x08F0, 0x08E4, 0x08D7, 0x08C9, 0x08BB, 0x08AC, 0x089D, 0x088D, 0x087D, 0x086C, 0x085A, 0x0848,
    0x0836, 0x0823, 0x080F, 0x07FB, 0x07E6, 0x07D1, 0x07BC, 0x07A6, 0x0790, 0x0779, 0x0762, 0x074A, 0x0732, 0x071A, 0x0701, 0x06E8,
    0x06CF, 0x06B5, 0x069B, 0x0681, 0x0667, 0x064C, 0x0631, 0x0616, 0x05FB, 0x05DF, 0x05C3, 0x05A8, 0x058C, 0x056F, 0x0553, 0x0537,
    0x051A, 0x04FE, 0x04E1, 0x04C5, 0x04A8, 0x048C, 0x046F, 0x0453, 0x0436, 0x041A, 0x03FE, 0x03E1, 0x03C5, 0x03A9, 0x038E, 0x0372,
    0x0357, 0x033B, 0x0320, 0x0305, 0x02EB, 0x02D1, 0x02B7, 0x029D, 0x0283, 0x026A, 0x0251, 0x0239, 0x0221, 0x0209, 0x01F2, 0x01DB,
    0x01C4, 0x01AE, 0x0198, 0x0183, 0x016E, 0x015A, 0x0146, 0x0133, 0x0120, 0x010E, 0x00FC, 0x00EB, 0x00DA, 0x00CA, 0x00BA, 0x00AB,
    0x009D, 0x008F, 0x0082, 0x0075, 0x0069, 0x005E, 0x0053, 0x0049, 0x003F, 0x0036, 0x002E, 0x0027, 0x0020, 0x0019, 0x0014, 0x000F,
    0x000B, 0x0007, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0003, 0x0006, 0x0009, 0x000D, 0x0011, 0x0017, 0x001C,
    0x0023, 0x002A, 0x0032, 0x003B, 0x0044, 0x004E, 0x0058, 0x0063, 0x006F, 0x007B, 0x0088, 0x0096, 0x00A4, 0x00B3, 0x00C2, 0x00D2,
    0x00E2, 0x00F3, 0x0105, 0x0117, 0x0129, 0x013C, 0x0150, 0x0164, 0x0179, 0x018E, 0x01A3, 0x01B9, 0x01CF, 0x01E6, 0x01FD, 0x0215,
    0x022D, 0x0245, 0x025E, 0x0277, 0x0290, 0x02AA, 0x02C4, 0x02DE, 0x02F8, 0x0313, 0x032E, 0x0349, 0x0364, 0x0380, 0x039C, 0x03B7,
    0x03D3, 0x03F0, 0x040C, 0x0428, 0x0445, 0x0461, 0x047E, 0x049A};

// SPWM初始化函数
void SpwmInit(void)
{
    TimA0_Int_Init(Tim0Period - 1, Tim0Prescaler); // 负责控制定时器A1输出SPWM
    TimA1_PWM_Init(Tim1Period - 1, Tim1Prescaler); // 负责输出PWM
}
/**************     SPWM波相关结束     **************/

/**************         TIMA0         **************/
void TimA0_Int_Init(uint16_t ccr0, uint16_t psc)
{
    /*增计数模式初始化*/
    Timer_A_UpModeConfig upConfig;
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //时钟源
    upConfig.clockSourceDivider = psc;                                                     //时钟分频 范围1-64
    upConfig.timerPeriod = ccr0;                                                           //自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //启用 ccr0更新中断
    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);                                 //选择定时器A1 TIMER_A0

    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE); //开启计数
    MAP_Timer_A_clearInterruptFlag(TIMER_A0_BASE);            //清空标志位
    MAP_Interrupt_enableInterrupt(INT_TA0_0);                 //开启定时器中断
}

void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0); // 清除中断标志位
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, (DutyCycleSineWaveTable[SineTablePos_1]) * MaxDutyCycle);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, (DutyCycleSineWaveTable[SineTablePos_2]) * MaxDutyCycle);
    MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, (DutyCycleSineWaveTable[SineTablePos_3]) * MaxDutyCycle);
    SineTablePos_1++;
    SineTablePos_2++;
    SineTablePos_3++;
    switch (Sector)
    {
    case 1:
        if (SineTablePos_3 == SamplesPerCycle)
        {
            SineTablePos_3 = 0;
            Sector = 2;
        }
        break;
    case 2:
        if (SineTablePos_1 == SamplesPerCycle)
        {
            SineTablePos_1 = 0;
            Sector = 3;
        }
        break;
    case 3:
        if (SineTablePos_2 == SamplesPerCycle)
        {
            SineTablePos_2 = 0;
            Sector = 1;
        }
        break;
    }
}

/**************         TIMA1         **************/
void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*初始化引脚*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7 + GPIO_PIN6 + GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);

    /*定时器PWM初始化*/
    Timer_A_PWMConfig TimA1_PWMConfig;
    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;           //时钟源
    TimA1_PWMConfig.clockSourceDivider = psc;                          //时钟分频 范围1-64
    TimA1_PWMConfig.timerPeriod = ccr0;                                //自动重装载值（ARR）
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET; //输出模式
    TimA1_PWMConfig.dutyCycle = ccr0;                                  //这里是改变占空比的地方 默认100%

    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; //通道一 （引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);            // 初始化比较寄存器以产生 PWM1
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_2; //通道二 （引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);            // 初始化比较寄存器以产生 PWM2
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_3; //通道三 （引脚定义）
    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);            // 初始化比较寄存器以产生 PWM3
}
