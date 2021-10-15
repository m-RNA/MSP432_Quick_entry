/****************************************************/
// MSP432P401R
// 8-1 ��ʱ���ж�
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/1
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "usart.h"
#include "led.h"

#define CLKDIV 64   //ʱ��Դ��Ƶ
#define CCR0 37499  // �Ƚ�ֵ0

/*
 * ��ʱ���ж����ڣ�
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk 
 *           = 64 * 37500 / 48000000 
 *           = 0.05s = 20Hz
 */
 
int main(void)
{
    SysInit();  			     // ��3�� ʱ������
	LED_Init();					 // ��2�� GPIO���
	TimA0_Int_Init(CCR0,CLKDIV); // ��8�� TIMA�ж�
    
    MAP_Interrupt_enableMaster(); // �������ж�
    while (1)
    {
    }
}


