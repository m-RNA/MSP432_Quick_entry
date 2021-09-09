#include "timA.h"

/****************************************************/
// MSP432P401R
// ��ʱ��A
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/26
/****************************************************/

uint16_t TIMA2_CAP_STA = 0;
uint16_t TIMA2_CAP_VAL = 0;

/*********************************************************************************************************/
/**************************************         TIMA2          *******************************************/

void TimA2_Cap_Init(void)
{
    /* ��ʱ�����ò���*/
    Timer_A_ContinuousModeConfig continuousModeConfig = {

        TIMER_A_CLOCKSOURCE_SMCLK,      // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_48, // SMCLK/48 = 1MHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,  // Enable Timer ISR
        TIMER_A_DO_CLEAR                // Skup Clear Counter
    };

    /* Timer_A ��׽ģʽ���ò��� */
    const Timer_A_CaptureModeConfig captureModeConfig_TA2 = {
        TIMER_A_CAPTURECOMPARE_REGISTER_1,           //CCR1�����������
        TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE, //�����ز���
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,           //CCIxA:�ⲿ��������  ��CCIxB:���ڲ�ACLK����(�ֲ�)
        TIMER_A_CAPTURE_SYNCHRONOUS,                 //ͬ������
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,     //�����ж�
        TIMER_A_OUTPUTMODE_OUTBITVALUE               //���λֵ
    };

    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION); //�����������

    Timer_A_initCapture(TIMER_A2_BASE, &captureModeConfig_TA2);
    Timer_A_configureContinuousMode(TIMER_A2_BASE, &continuousModeConfig);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

    Timer_A_clearInterruptFlag(TIMER_A2_BASE);                                              //��ʱ�������ж�
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1); //CCR1�����ж�

    Interrupt_enableInterrupt(INT_TA2_N);
}

void TA2_N_IRQHandler(void)
{
    if ((TIMA2_CAP_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if (Timer_A_getEnabledInterruptStatus(TIMER_A2_BASE)) //����ж�
        {
			Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            Timer_A_clearInterruptFlag(TIMER_A2_BASE);
            BITBAND_PERI(TIMER_A_CMSIS(TIMER_A2_BASE)->CCTL[1], TIMER_A_CCTLN_COV_OFS) = 0; //�����λCOV

            if (TIMA2_CAP_STA & 0X40) //�Ѿ����񵽸ߵ�ƽ�� 4 0100
            {
                if ((TIMA2_CAP_STA & 0X3F) == 0X3F) //�ߵ�ƽ̫����
                {
                    TIMA2_CAP_STA |= 0X80; //ǿ�Ʊ�ǳɹ�������ߵ�ƽ 8 = 0x1000
                    TIMA2_CAP_VAL = 0XFFFF;
                }
                else
                    TIMA2_CAP_STA++; //���������һ
            }
        }

        if (Timer_A_getCaptureCompareEnabledInterruptStatus(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1))
        {
            Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            if (Timer_A_getSynchronizedCaptureCompareInput(TIMER_A2_BASE,
                                                           TIMER_A_CAPTURECOMPARE_REGISTER_1,
                                                           TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_LOW)
            {
                TIMA2_CAP_STA |= 0X80; //��ǳɹ�������ߵ�ƽ
                TIMA2_CAP_VAL = Timer_A_getCaptureCompareCount(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
            }
            else //��δ��ʼ,��һ�β���������
            {
                TIMA2_CAP_STA = 0; //���
                TIMA2_CAP_VAL = 0;
                Timer_A_clearTimer(TIMER_A2_BASE);
                TIMA2_CAP_STA |= 0X40; //��ǲ�����������
            }
        }
    }
}
/*********************************************************************************************************/

/*********************************************************************************************************/
/**************************************         TIMA1          *******************************************/

void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc)
{
    /*��ʼ������*/
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P7, GPIO_PIN7, GPIO_PRIMARY_MODULE_FUNCTION);

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
