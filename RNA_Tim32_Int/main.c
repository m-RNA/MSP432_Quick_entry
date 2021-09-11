#include "sysinit.h"
#include "uartinit.h"
#include "led.h"
#include "tim32.h"

/****************************************************/
// MSP432P401R
// 9 定时器32 中断
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/8
/****************************************************/

/*
 * 定时器中断周期：
 *
 * T_timer_32 = CLKDIV * (CCR0 + 1) / f_clk 
 *            = 256 * 187500 / 48000000 
 *            = 1s = 1Hz
 */

#define CLKDIV TIMER32_PRESCALER_256 // 时钟源分频
#define ARR 187500                   // 自动重装载值

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
