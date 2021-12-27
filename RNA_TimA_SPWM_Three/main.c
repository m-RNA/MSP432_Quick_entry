/****************************************************/
// MSP432P401R
// ��8�� ��ʱ��A ��·SPWM���
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/26
/****************************************************/

/*******************     SPWM x3      ****************

    ��ʱ��˵����
	T0  �ж�  ������ʱ��TimA1�����PWM��ΪSPWM
	T1  PWM	 ����ռ�� P7.5  P7.6  P7.7

    ����˵����
    KEY1 �ı�Ƶ��
    KEY2 �ı��ֵ

******************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "timA.h"
#include "key.h"
#include "led.h"

int main(void)
{
    volatile uint8_t i = 0;
    volatile uint8_t key = 0; // ��ֵ

    SysInit();         // ��3�� ʱ������
    uart_init(115200); // ��7�� ��������
    delay_init();      // ��4�� �δ���ʱ
    LED_Init();        // ��2�� GPIO���
    KEY_Init(0);       // ��2�� GPIO����
    SpwmInit();        // ��8�� SPWM���

    /* ��ӡ��ʼ��Ϣ */
    printf("Hello MSP423!\r\n");
    printf("Frequency:%dHz\r\n", FundamentalFrequency);
    printf("MaxDutyCycle:%f\r\n", MaxDutyCycle);

    MAP_Interrupt_enableMaster();
    while (1)
    {
        key = KEY_Scan(0); // ɨ�谴��
        if (key == 1)      // ����KEY1�ı����������Ƶ��
        {
            // ����Ƶ��Ϊ 25~100Hz
            if (FundamentalFrequency == 100) //
            {
                FundamentalFrequency = 25;
            }
            else
            {
                FundamentalFrequency += 1;
            }
            // �ı䶨ʱ��A0�Ŀ���Ƶ�ʣ����ı������SPWM�Ļ���Ƶ��
            MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, 48000000 / (SamplesPerCycle * FundamentalFrequency));
            printf("Frequency:%dHz\r\n", FundamentalFrequency);
        }
        else if (key == 2) // ����KEY1�ı���������ķ�ֵ
        {
            // �������ռ�ձ�Ϊ 50~95%
            if (MaxDutyCycle == 0.95f)
            {
                MaxDutyCycle = 0.5f;
            }
            else
            {
                MaxDutyCycle += 0.05f;
            }
            printf("MaxDutyCycle:%f\r\n", MaxDutyCycle);
        }
        key = 0;
        delay_ms(20);

        /* ��˸���غ����ʾ�⹤���� */
        if (i == 10)
        {
            i = 0;
            LED_RED_Tog(); // ��˸���
        }
        else
        {
            ++i;
        }
    }
}
