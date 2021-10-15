/****************************************************/
// MSP432P401R
// 8-2 ��ʱ��A PWMģʽ �������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/1
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "timA.h"

/*
 * ��ʱ��PWM���ڣ�
 *`   
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk
 *           = 48 * ��19999 + 1�� / 48000000
 *           = 0.02s = 50Hz
 */

#define CLKDIV 48     // ʱ��Դ��Ƶ
#define CCR0 19999    // �Ƚ�ֵ0
#define CCR1_MIN 499  // ( 499 + 1) / (19999 + 1) =  500 / 20000 =  2.5%
#define CCR1_MAX 2499 // (2499 + 1) / (19999 + 1) = 2500 / 20000 = 12.5%

int main(void)
{
    bool dir = 1;
    uint16_t i = CCR1_MIN;

    SysInit();    //��3�� ʱ������
    delay_init(); //��4�� �δ���ʱ
	
    TimA1_PWM_Init(CCR0, CLKDIV); //��8�� ��ʱ��A PWM
    while (1)
    {
        if (dir)
            i++;
        else
            i--;

        if (i == CCR1_MAX)
        {
            dir = 0;
            delay_ms(50);
        }
        else if (i == CCR1_MIN)
        {
            dir = 1;
            delay_ms(50);
        }
        MAP_Timer_A_setCompareValue(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1, i);
        delay_us(600);
    }
}
