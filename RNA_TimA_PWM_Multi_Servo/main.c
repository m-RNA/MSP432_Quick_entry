/****************************************************/
// MSP432P401R
// 8-1 ��ʱ�� ��·PWM��� 50Hz ���
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/3
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "delay.h"

/*
 * ��ʱ��PWM���ڣ�
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk
 *           = 48 * ��19999 + 1�� / 48000000
 *           = 0.02s = 50Hz
 */

#define CCR0 20000 - 1
#define CLKDIV TIMER_A_CLOCKSOURCE_DIVIDER_48
#define CCRN_MIN 499  // ( 499 + 1) / (19999 + 1) =  500 / 20000 =  2.5%
#define CCRN_MAX 2499 // (2499 + 1) / (19999 + 1) = 2500 / 20000 = 12.5%

/***********   ɨ��4·���  ***********/

int main(void)
{
    bool dir = 1;
    uint16_t i = CCRN_MIN;
    uint16_t CCR1;
    uint16_t CCR2;
    uint16_t CCR3;
    uint16_t CCR4;

    SysInit();  				  //��3�� ʱ������
    delay_init();				  //��4�� �δ���ʱ
    TimA1_PWM_Init(CCR0, CLKDIV); //��8�� ��ʱ��A PWM
	
    while (1)
    {
        if (dir)
        {
            ++i;
        }
        else
        {
            --i;
        }

        if (i == CCRN_MAX)
        {
            dir = 0;
            delay_ms(50);
        }
        else if (i == CCRN_MIN)
        {
            dir = 1;
            delay_ms(50);
        }

        CCR1 = i;
        CCR2 = i / 2;
        CCR3 = i / 3;
        CCR4 = i / 4;

        /******************      ɨ��4·���      ******************/
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, CCR1); //ͨ�����Ĵ����CCR1���ı�ͨ��1ռ�ձ�
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, CCR2); //ͨ�����Ĵ����CCR2���ı�ͨ��2ռ�ձ�
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, CCR3); //ͨ�����Ĵ����CCR3���ı�ͨ��3ռ�ձ�
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, CCR4); //ͨ�����Ĵ����CCR4���ı�ͨ��4ռ�ձ�
        delay_ms(50);
    }
}
