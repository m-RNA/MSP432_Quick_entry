/****************************************************/
// MSP432P401R
// 9* 定时器32 软件扩展定时器（跟随开发更新）
// 项目地址：https://github.com/0x1abin/MultiTimer
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/13
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "led.h"
#include "tim32.h"
#include "MultiTimer.h"

MultiTimer timer1;
MultiTimer timer2;
MultiTimer timer3;
MultiTimer timer4;
MultiTimer timer5;

void exampleTimer1Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", MultiTimerGetValue(), timer, (char *)userData);
}

void exampleTimer2Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", MultiTimerGetValue(), timer, (char *)userData);
    MultiTimerStart(timer, 5000, exampleTimer2Callback, userData);
}

void exampleTimer3Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", MultiTimerGetValue(), timer, (char *)userData);
    MultiTimerStart(timer, 4567, exampleTimer3Callback, userData);
}

void exampleTimer4Callback(MultiTimer *timer, void *userData)
{
    LED_W_Tog();
    MultiTimerStart(&timer5, 500, exampleTimer4Callback, NULL);
}

int main(void)
{
    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    LED_Init();        // 第2讲 GPIO输出
    MultiTimerInit();  // 第10讲 软件定时器

    MultiTimerStart(&timer1, 1000, exampleTimer1Callback, "1000ms ONCE timer"); //单次中断
    MultiTimerStart(&timer2, 5000, exampleTimer2Callback, "5000ms CYCLE timer");//循环中断
    MultiTimerStart(&timer3, 3456, exampleTimer3Callback, "3456ms delay start, 4567ms CYCLE timer");
    MultiTimerStart(&timer4, 0, exampleTimer4Callback, NULL); //立即中断

    MAP_Interrupt_enableMaster(); // 开启总中断
    while (1)
    {
        MultiTimerYield();
        LED_RED_Tog();
        delay_ms(50);
    }
}
