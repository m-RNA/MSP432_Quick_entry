/****************************************************/
// MSP432P401R
// 8-1 定时器 多路PWM输出 50Hz 舵机
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/3
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "delay.h"

/*
 * 定时器PWM周期：
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk
 *           = 48 * （19999 + 1） / 48000000
 *           = 0.02s = 50Hz
 */

#define CCR0 20000 - 1
#define CLKDIV TIMER_A_CLOCKSOURCE_DIVIDER_48
#define CCRN_MIN 499  // ( 499 + 1) / (19999 + 1) =  500 / 20000 =  2.5%
#define CCRN_MAX 2499 // (2499 + 1) / (19999 + 1) = 2500 / 20000 = 12.5%

/***********   扫描4路舵机  ***********/

int main(void)
{
    bool dir = 1;
    uint16_t i = CCRN_MIN;
    uint16_t CCR1;
    uint16_t CCR2;
    uint16_t CCR3;
    uint16_t CCR4;

    SysInit();  				  //第3讲 时钟配置
    delay_init();				  //第4讲 滴答延时
    TimA1_PWM_Init(CCR0, CLKDIV); //第8讲 定时器A PWM
	
    while (1)
    {
        if (dir)
        {
            ++i;
        }
        else
        {
            --i;
        }

        if (i == CCRN_MAX)
        {
            dir = 0;
            delay_ms(50);
        }
        else if (i == CCRN_MIN)
        {
            dir = 1;
            delay_ms(50);
        }

        CCR1 = i;
        CCR2 = i / 2;
        CCR3 = i / 3;
        CCR4 = i / 4;

        /******************      扫描4路舵机      ******************/
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, CCR1); //通过更改传入的CCR1来改变通道1占空比
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, CCR2); //通过更改传入的CCR2来改变通道2占空比
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, CCR3); //通过更改传入的CCR3来改变通道3占空比
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, CCR4); //通过更改传入的CCR4来改变通道4占空比
        delay_ms(50);
    }
}
