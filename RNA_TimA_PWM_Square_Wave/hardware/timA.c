#include "timA.h"

/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

/*********************************************************************************************************/
/**************************************         TIMA1          *******************************************/
void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*初始化引脚*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

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
