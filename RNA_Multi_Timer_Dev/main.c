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

extern uint32_t platformTicksFunction(void);

void exampleTimer1Callback(MultiTimer* timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", platformTicksFunction(), timer, (char*)userData);
    MultiTimerStart(timer, 1000, exampleTimer1Callback, userData);
}

void exampleTimer2Callback(MultiTimer* timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", platformTicksFunction(), timer, (char*)userData);
}

void exampleTimer3Callback(MultiTimer* timer, void *userData)
{
    printf("[T:%010d] Timer:%p callback-> %s.\r\n", platformTicksFunction(), timer, (char*)userData);
    MultiTimerStart(timer, 4567, exampleTimer3Callback, userData);

}

void exampleTimer4Callback(MultiTimer* timer, void *userData)
{
    LED_RED_Tog();
    MultiTimerStart(&timer4, 1000, exampleTimer4Callback, NULL);
}

void exampleTimer5Callback(MultiTimer* timer, void *userData)
{
    LED_W_Tog();
    MultiTimerStart(&timer5, 500, exampleTimer5Callback, NULL);
 }

int main(void)
{
    SysInit();                     // 第3讲 时钟配置
    uart_init(115200);             // 第7讲 串口配置
    LED_Init();

    MultiTimerInit();

    MultiTimerStart(&timer1, 1000, exampleTimer1Callback, "1000ms CYCLE timer");
    MultiTimerStart(&timer2, 5000, exampleTimer2Callback, "5000ms ONCE timer");
    MultiTimerStart(&timer3, 3456, exampleTimer3Callback, "3456ms delay start, 4567ms CYCLE timer");
	
    MultiTimerStart(&timer4, 1000, exampleTimer4Callback, NULL);
    MultiTimerStart(&timer5, 500, exampleTimer5Callback, NULL);

    while (1)
    {
        MultiTimerYield();
    }
}
