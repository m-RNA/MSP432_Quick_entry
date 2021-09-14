#include "tim32.h"
#include "uartinit.h"

/****************************************************/
// MSP432P401R
// 定时器32
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/8
/****************************************************/

void Tim32_0_Int_Init(uint32_t arr, uint8_t psc)
{
    // 1.初始化定时器32 为 32位周期计数模式
    MAP_Timer32_initModule(TIMER32_0_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    // 2.设置ARR自动重装载值
    MAP_Timer32_setCount(TIMER32_0_BASE, arr);

    // 3.使能定时器32_0中断
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);

    // 4.配置定时器32开始连续计数 false
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    // 5.开启定时器32端口中断
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
}

/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    //清除中断标志位
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    /*开始填充用户代码*/
    static uint8_t timer_second = 0;

    //一般在频率较高的中断不常用 这个printf比较费时间 这里只是演示
    printf("%d秒过去了\r\n\r\n", ++timer_second);

    /*结束填充用户代码*/
}
