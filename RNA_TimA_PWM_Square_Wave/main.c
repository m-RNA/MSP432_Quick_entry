#include "sysinit.h"
#include "uartinit.h"
#include "timA.h"
#include "uartinit.h"
#include "led.h"

/****************************************************/
// MSP432P401R
// 8-1 ��ʱ�� PWM��� 50Hz ����
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/3
/****************************************************/

/*
 * ��ʱ��PWM���ڣ�
 *
 * T_timer_a = CLKDIV * (CCR0 + 1) / f_clk 
 *           = 48 * ��19999 + 1�� / 48000000 
 *           = 0.02s = 50Hz
 */

int main(void)
{
    // 0.����ʱ��
    SysInit();

    // 1.����GPIO����
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

    // 2.���ýṹ��
    Timer_A_PWMConfig TimA1_PWMConfig;
    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //ʱ��Դ
    TimA1_PWMConfig.clockSourceDivider = 48;                             //ʱ�ӷ�Ƶ ��Χ1-64
    TimA1_PWMConfig.timerPeriod = 19999;                                 //�Զ���װ��ֵ��ARR��
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; //ͨ��һ �����Ŷ��壩
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //���ģʽ
    TimA1_PWMConfig.dutyCycle = 9999;                                    //�����Ǹı�ռ�ձȵĵط�

    // 3.��ʼ����ʱ��
    Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig);

    while (1)
    {
    }
}
