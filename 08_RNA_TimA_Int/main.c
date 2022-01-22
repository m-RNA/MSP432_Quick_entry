/****************************************************/
// MSP432P401R
// 8-1 定时器中断
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/1
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "usart.h"
#include "led.h"

#define CLKDIV 64   //时钟源分频
#define CCR0 37499  // 比较值0

/*
 * 定时器中断周期：
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk 
 *           = 64 * 37500 / 48000000 
 *           = 0.05s = 20Hz
 */
 
int main(void)
{
    SysInit();  			     // 第3讲 时钟配置
	LED_Init();					 // 第2讲 GPIO输出
	TimA0_Int_Init(CCR0,CLKDIV); // 第8讲 TIMA中断
    
    MAP_Interrupt_enableMaster(); // 开启总中断
    while (1)
    {
    }
}


