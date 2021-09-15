#include "tim32.h"
#include "uartinit.h"
#include "multi_timer.h"

/****************************************************/
// MSP432P401R
// ��ʱ��32
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/8
/****************************************************/

void Tim32_0_Int_Init(uint32_t aar, uint8_t psc)
{
    MAP_Timer32_initModule(TIMER32_0_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_0_BASE, aar);

    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);

    MAP_Timer32_startTimer(TIMER32_0_BASE, false); //��������ģʽ false

    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
}

/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    /*��ʼ����û�����*/
    static uint8_t timer_second = 0;

    //һ����Ƶ�ʽϸߵ��жϲ����� ���printf�ȽϷ�ʱ�� ����ֻ����ʾ
    printf("%d���ȥ��\r\n\r\n", ++timer_second);

    /*��������û�����*/
}
void Tim32_1_Int_Init(uint32_t aar, uint8_t psc)
{
    MAP_Timer32_initModule(TIMER32_1_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    MAP_Timer32_setCount(TIMER32_1_BASE, aar);

    MAP_Timer32_enableInterrupt(TIMER32_1_BASE);

    MAP_Timer32_startTimer(TIMER32_1_BASE, false); //��������ģʽ false

    MAP_Interrupt_enableInterrupt(INT_T32_INT2);
}

/* Timer32 ISR */
void T32_INT2_IRQHandler(void)
{
    MAP_Timer32_clearInterruptFlag(TIMER32_1_BASE);

    /*��ʼ����û�����*/

    timer_ticks(); //1ms ticks

    /*��������û�����*/
}