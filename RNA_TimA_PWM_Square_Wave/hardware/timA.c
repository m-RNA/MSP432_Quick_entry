#include "timA.h"

/****************************************************/
// MSP432P401R
// ��ʱ��A
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/26
/****************************************************/

/*********************************************************************************************************/
/**************************************         TIMA1          *******************************************/
void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*��ʼ������*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);

    Timer_A_PWMConfig TimA1_PWMConfig;
    /*��ʱ��PWM��ʼ��*/
    TimA1_PWMConfig.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;             //ʱ��Դ
    TimA1_PWMConfig.clockSourceDivider = psc;                            //ʱ�ӷ�Ƶ ��Χ1-64
    TimA1_PWMConfig.timerPeriod = ccr0;                                  //�Զ���װ��ֵ��ARR��
    TimA1_PWMConfig.compareRegister = TIMER_A_CAPTURECOMPARE_REGISTER_1; //ͨ��һ �����Ŷ��壩
    TimA1_PWMConfig.compareOutputMode = TIMER_A_OUTPUTMODE_TOGGLE_SET;   //���ģʽ
    TimA1_PWMConfig.dutyCycle = ccr0;                                    //�����Ǹı�ռ�ձȵĵط� Ĭ��100%

    MAP_Timer_A_generatePWM(TIMER_A1_BASE, &TimA1_PWMConfig); /* ��ʼ���ȽϼĴ����Բ��� PWM1 */
}
/*********************************************************************************************************/

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
