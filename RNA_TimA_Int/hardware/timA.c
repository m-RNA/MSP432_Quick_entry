#include "timA.h"

/****************************************************/
// MSP432P401R
// ��ʱ��A
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/26
/****************************************************/

/*********************************************************************************************************/
/**************************************         TIMA0          *******************************************/

void TimA0_Int_Init(uint16_t ccr0, uint16_t psc)
{
    // 1.������ģʽ��ʼ��
    Timer_A_UpModeConfig upConfig;
    upConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;                                      //ʱ��Դ
    upConfig.clockSourceDivider = psc;                                                     //ʱ�ӷ�Ƶ ��Χ1-64
    upConfig.timerPeriod = ccr0;                                                           //�Զ���װ��ֵ��ARR��
    upConfig.timerInterruptEnable_TAIE = TIMER_A_TAIE_INTERRUPT_DISABLE;                   //���� tim����ж�
    upConfig.captureCompareInterruptEnable_CCR0_CCIE = TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE; //���� ccr0�����ж�
    upConfig.timerClear = TIMER_A_DO_CLEAR;                                                // Clear value

    // 2.��ʼ����ʱ��A
    MAP_Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);

    // 3.ѡ��ģʽ��ʼ����
    MAP_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);

    // 4.����Ƚ��жϱ�־λ
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    // 5.�������ڶ˿��ж�
    MAP_Interrupt_enableInterrupt(INT_TA0_0);
}

// 6.��дTIMA ISR
void TA0_0_IRQHandler(void)
{
    MAP_Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
	
	/*��ʼ����û�����*/
	
    MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
	
	/*��������û�����*/

}

/*********************************************************************************************************/
