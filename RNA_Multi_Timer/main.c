#include "sysinit.h"
#include "uartinit.h"
#include "led.h"
#include "tim32.h"
#include "multi_timer.h"

/****************************************************/
// MSP432P401R
// 9* 定时器32 软件无限扩展定时器（master）
// 项目地址：https://github.com/0x1abin/MultiTimer
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/13
/****************************************************/

/*
 * 定时器中断周期：
 *
 * T_timer_32 = CLKDIV * (ARR + 1) / f_clk
 *            = 1 * 48000 / 48000000
 *            = 1ms = 1000Hz
 */

#define CLKDIV TIMER32_PRESCALER_1 // 时钟源分频
#define ARR 47999                  // 自动重装载值

struct Timer timer1;
struct Timer timer2;
struct Timer timer3;

void timer1_callback(void *arg)
{
    LED_RED_Tog();
}

void timer2_callback(void *arg)
{
    printf("timer2 timeout! arg: %p\r\n", arg);
}

void timer3_callback(void *arg)
{
    printf("timer3 timeout! arg: %p\r\n", arg);
}

int main(void)
{
    SysInit();                     // 第3讲 时钟配置
    uart_init(115200);             // 第7讲 串口配置
    Tim32_1_Int_Init(ARR, CLKDIV); // 第9讲 TIM32中断
	
    LED_Init();

    timer_init(&timer1, timer1_callback, 50, 50, NULL);     //  50ms 循环
    timer_init(&timer2, timer2_callback, 1000, 1000, NULL); //    1s 循环
    timer_init(&timer3, timer3_callback, 1234, 0, NULL);    //1.234s 一次

    timer_start(&timer1);
    timer_start(&timer2);
    timer_start(&timer3);

    MAP_Interrupt_enableMaster(); // 开启总中断

    while (1)
    {
        timer_loop();
    }
}
