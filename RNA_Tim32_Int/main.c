/****************************************************/
// MSP432P401R
// 9 定时器32 中断
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/8
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "led.h"
#include "tim32.h"

/*
 * 定时器中断周期：
 *
 * T_timer_32 = CLKDIV * (ARR + 1) / f_clk 
 *            = 1 * 48000000 / 48000000 
 *            = 1s = 1Hz
 */

#define CLKDIV TIMER32_PRESCALER_1 // 时钟源分频
#define ARR 47999999               // 自动重装载值

int main(void)
{
    SysInit();                     // 第3讲 时钟配置
    uart_init(115200);             // 第7讲 串口配置
	
    Tim32_0_Int_Init(ARR, CLKDIV); // 第9讲 TIM32中断
	
	printf("砸瓦鲁多\r\n\r\n");

    MAP_Interrupt_enableMaster(); // 开启总中断

    while (1)
    {
    }
}
