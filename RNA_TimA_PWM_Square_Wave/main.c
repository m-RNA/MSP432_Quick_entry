#include "sysinit.h"
#include "uartinit.h"
#include "timA.h"
#include "uartinit.h"
#include "led.h"

/****************************************************/
// MSP432P401R
// 8-1 定时器 PWM输出 50Hz 方波
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/3
/****************************************************/

/*
 * 定时器PWM周期：
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk 
 *           = 48 * （19999 + 1） / 48000000 
 *           = 0.02s = 50Hz
 */

int main(void)
{
    // 0.配置时钟
    SysInit();

    // 1.配置GPIO复用
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    // 2.配置结构体
    Timer_A_PWMConfig TimA1_PWMConfig;
    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //时钟源
    TimA1_PWMConfig.clockSourceDivider = 48;                             //时钟分频 范围1-64
    TimA1_PWMConfig.timerPeriod = 19999;                                 //自动重装载值（ARR）
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; //通道一 （引脚定义）
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //输出模式
    TimA1_PWMConfig.dutyCycle = 9999;                                    //这里是改变占空比的地方

    // 3.初始化定时器
    Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);

    while (1)
    {
    }
}
