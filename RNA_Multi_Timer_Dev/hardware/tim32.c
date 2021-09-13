#include "tim32.h"
#include "uartinit.h"
#include "MultiTimer.h"

/****************************************************/
// MSP432P401R
// 定时器32
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/8
/****************************************************/

void Tim32_0_Int_Init(uint32_t aar, uint8_t psc)
{
    MAP_Timer32_initModule(TIMER32_0_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_0_BASE, aar);

    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);

    MAP_Timer32_startTimer(TIMER32_0_BASE, false); //连续计数模式 false

    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
}

/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    /*开始填充用户代码*/
    static uint8_t timer_second = 0;

    //一般在频率较高的中断不常用 这个printf比较费时间 这里只是演示
    printf("%d秒过去了\r\n\r\n", ++timer_second);

    /*结束填充用户代码*/
}
void Tim32_1_Int_Init(uint32_t aar, uint8_t psc)
{
    MAP_Timer32_initModule(TIMER32_1_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_1_BASE, aar);

    MAP_Timer32_enableInterrupt(TIMER32_1_BASE);

    MAP_Timer32_startTimer(TIMER32_1_BASE, false); //连续计数模式 false

    MAP_Interrupt_enableInterrupt(INT_T32_INT2);
}

/* Timer32 ISR */
void T32_INT2_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE);

    /*开始填充用户代码*/

    MultiTimerTicks(); //1ms ticks

    /*结束填充用户代码*/
}
