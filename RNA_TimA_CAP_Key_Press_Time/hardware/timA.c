#include "timA.h"

/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

uint16_t TIMA2_CAP_STA = 0;
uint16_t TIMA2_CAP_VAL = 0;

/*********************************************************************************************************/
/**************************************         TIMA2          *******************************************/

void TimA2_Cap_Init(void)
{
    /* 定时器配置参数*/
    Timer_A_ContinuousModeConfig continuousModeConfig = {

        TIMER_A_CLOCKSOURCE_SMCLK,      // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_48, // SMCLK/48 = 1MHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,  // Enable Timer ISR
        TIMER_A_DO_CLEAR                // Skup Clear Counter
    };

    /* Timer_A 捕捉模式配置参数 */
    const Timer_A_CaptureModeConfig captureModeConfig_TA2 = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,           //CCR1在这里改引脚
        TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE, //上升沿捕获
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,           //CCIxA:外部引脚输入  （CCIxB:与内部ACLK连接(手册)
        TIMER_A_CAPTURE_SYNCHRONOUS,                 //同步捕获
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,     //开启中断
        TIMER_A_OUTPUTMODE_OUTBITVALUE               //输出位值
    };

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //在这里改引脚

    Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig_TA2);
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

    Timer_A_clearInterruptFlag(TIMER_A2_BASE);                                              //定时器更新中断
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1); //CCR1更新中断

    Interrupt_enableInterrupt(INT_TA2_N);
}

void TA2_N_IRQHandler(void)
{
    if ((TIMA2_CAP_STA & 0X80) == 0) //还未成功捕获
    {
        if (Timer_A_getEnabledInterruptStatus(TIMER_A2_BASE)) //溢出中断
        {
			Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            Timer_A_clearInterruptFlag(TIMER_A2_BASE);
            BITBAND_PERI(TIMER_A_CMSIS(TIMER_A2_BASE)->CCTL[1], TIMER_A_CCTLN_COV_OFS) = 0; //软件复位COV

            if (TIMA2_CAP_STA & 0X40) //已经捕获到高电平了 4 0100
            {
                if ((TIMA2_CAP_STA & 0X3F) == 0X3F) //高电平太长了
                {
                    TIMA2_CAP_STA |= 0X80; //强制标记成功捕获完高电平 8 = 0x1000
                    TIMA2_CAP_VAL = 0XFFFF;
                }
                else
                    TIMA2_CAP_STA++; //溢出次数加一
            }
        }

        if (Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1))
        {
            Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            if (Timer_A_getSynchronizedCaptureCompareInput(TIMER_A2_BASE,
                                                           TIMER_A_CAPTURECOMPARE_REGISTER_1,
                                                           TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_LOW)
            {
                TIMA2_CAP_STA |= 0X80; //标记成功捕获完高电平
                TIMA2_CAP_VAL = Timer_A_getCaptureCompareCount(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            }
            else //还未开始,第一次捕获上升沿
            {
                TIMA2_CAP_STA = 0; //清空
                TIMA2_CAP_VAL = 0;
                Timer_A_clearTimer(TIMER_A2_BASE);
                TIMA2_CAP_STA |= 0X40; //标记捕获到了上升沿
            }
        }
    }
}
/*********************************************************************************************************/

/*********************************************************************************************************/
/**************************************         TIMA1          *******************************************/

void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*初始化引脚*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_PWMConfig TimA1_PWMConfig;
    /*定时器PWM初始化*/
    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //时钟源
    TimA1_PWMConfig.clockSourceDivider = psc;                            //时钟分频 范围1-64
    TimA1_PWMConfig.timerPeriod = ccr0;                                  //自动重装载值（ARR）
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; //通道一 （引脚定义）
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //输出模式
    TimA1_PWMConfig.dutyCycle = ccr0;                                    //这里是改变占空比的地方 默认100%

    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig); /* 初始化比较寄存器以产生 PWM1 */
}
/*********************************************************************************************************/

/*********************************************************************************************************/
/**************************************         TIMA0          *******************************************/

void TimA0_Int_Init(uint16_t ccr0, uint16_t psc)
{
    // 1.增计数模式初始化
    Timer_A_UpModeConfig upConfig;
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //时钟源
    upConfig.clockSourceDivider = psc;                                                     //时钟分频 范围1-64
    upConfig.timerPeriod = ccr0;                                                           //自动重装载值（ARR）
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //禁用 tim溢出中断
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //启用 ccr0更新中断
    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value

    // 2.初始化定时器A
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);

    // 3.选择模式开始计数
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // 4.清除比较中断标志位
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // 5.开启串口端口中断
    MAP_Interrupt_enableInterrupt(INT_TA0_0);
}

// 6.编写TIMA ISR
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    /*开始填充用户代码*/

    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);

    /*结束填充用户代码*/
}
/*********************************************************************************************************/
