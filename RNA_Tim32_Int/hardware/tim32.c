#include "tim32.h"
#include "uartinit.h"

/****************************************************/
// MSP432P401R
// ��ʱ��32
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/8
/****************************************************/

void Tim32_0_Int_Init(uint32_t arr, uint8_t psc)
{
    // 1.��ʼ����ʱ��32 Ϊ 32λ���ڼ���ģʽ
    MAP_Timer32_initModule(TIMER32_0_BASE, psc, TIMER32_32BIT, TIMER32_PERIODIC_MODE);

    // 2.����ARR�Զ���װ��ֵ
    MAP_Timer32_setCount(TIMER32_0_BASE, arr);

    // 3.ʹ�ܶ�ʱ��32_0�ж�
    MAP_Timer32_enableInterrupt(TIMER32_0_BASE);

    // 4.���ö�ʱ��32��ʼ�������� false
    MAP_Timer32_startTimer(TIMER32_0_BASE, false);

    // 5.������ʱ��32�˿��ж�
    MAP_Interrupt_enableInterrupt(INT_T32_INT1);
}

/* Timer32 ISR */
void T32_INT1_IRQHandler(void)
{
    //����жϱ�־λ
    MAP_Timer32_clearInterruptFlag(TIMER32_0_BASE);

    /*��ʼ����û�����*/
    static uint8_t timer_second = 0;

    //һ����Ƶ�ʽϸߵ��жϲ����� ���printf�ȽϷ�ʱ�� ����ֻ����ʾ
    printf("%d���ȥ��\r\n\r\n", ++timer_second);

    /*��������û�����*/
}
