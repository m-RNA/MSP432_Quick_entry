/****************************************************/
// MSP432P401R
// 9 ��ʱ��32 �ж�
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/8
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "led.h"
#include "tim32.h"

/*
 * ��ʱ���ж����ڣ�
 *
 * T_timer_32 = CLKDIV * (ARR + 1) / f_clk 
 *            = 1 * 48000000 / 48000000 
 *            = 1s = 1Hz
 */

#define CLKDIV TIMER32_PRESCALER_1 // ʱ��Դ��Ƶ
#define ARR 47999999               // �Զ���װ��ֵ

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
