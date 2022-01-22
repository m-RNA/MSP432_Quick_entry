/****************************************************/
// MSP432P401R
// 8-2 定时器A PWM模式 驱动舵机
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/1
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "timA.h"

/*
 * 定时器PWM周期：
 *`   
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk
 *           = 48 * （19999 + 1） / 48000000
 *           = 0.02s = 50Hz
 */

#define CLKDIV 48     // 时钟源分频
#define CCR0 19999    // 比较值0
#define CCR1_MIN 499  // ( 499 + 1) / (19999 + 1) =  500 / 20000 =  2.5%
#define CCR1_MAX 2499 // (2499 + 1) / (19999 + 1) = 2500 / 20000 = 12.5%

int main(void)
{
    bool dir = 1;
    uint16_t i = CCR1_MIN;

    SysInit();    //第3讲 时钟配置
    delay_init(); //第4讲 滴答延时
	
    TimA1_PWM_Init(CCR0, CLKDIV); //第8讲 定时器A PWM
    while (1)
    {
        if (dir)
            i++;
        else
            i--;

        if (i == CCR1_MAX)
        {
            dir = 0;
            delay_ms(50);
        }
        else if (i == CCR1_MIN)
        {
            dir = 1;
            delay_ms(50);
        }
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, i);
        delay_us(600);
    }
}
