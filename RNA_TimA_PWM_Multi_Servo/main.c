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

    SysInit();    //��3�� ʱ������
    delay_init(); //��4�� �δ���ʱ

    TimA1_PWM_Init(CCR0, CLKDIV); //��8�� ��ʱ��A PWM
    while (1)
    {
        if (dir)
            ++i;
        else
            --i;

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
        /******************      ɨ��4·���      ******************/
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, i);
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_2, i);
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_3, i);
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, i);
        delay_ms(50);
    }
