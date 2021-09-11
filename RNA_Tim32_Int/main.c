#include "sysinit.h"
#include "uartinit.h"
#include "led.h"
#include "tim32.h"

/****************************************************/
// MSP432P401R
// 9 ��ʱ��32 �ж�
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/8
/****************************************************/

/*
 * ��ʱ���ж����ڣ�
 *
 * T_timer_32 = CLKDIV * (CCR0 + 1) / f_clk 
 *            = 256 * 187500 / 48000000 
 *            = 1s = 1Hz
 */

#define CLKDIV TIMER32_PRESCALER_256 // ʱ��Դ��Ƶ
#define ARR 187500                   // �Զ���װ��ֵ

int main(void)
{
    SysInit();                     // ��3�� ʱ������
    uart_init(115200);             // ��7�� ��������
	
    Tim32_0_Int_Init(ARR, CLKDIV); // ��9�� TIM32�ж�
	
	printf("����³��\r\n\r\n");

    MAP_Interrupt_enableMaster(); // �������ж�

    while (1)
    {
    }
}
