#include "timA.h"

/****************************************************/
// MSP432P401R
// ��ʱ��A
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/26
/****************************************************/

/*********************************************************************************************************/
/**************************************         TIMA2          *******************************************/

#define CAP_TIMA_SELECTION TIMER_A2_BASE                         //������Ķ�ʱ��
#define CAP_REGISTER_SELECTION TIMER_A_CAPTURECOMPARE_REGISTER_1 //������Ķ�ʱ��ͨ��
#define CAP_CCR_NUM 1                                            //������Ķ�ʱ��ͨ��
#define CAP_PORT_PIN GPIO_PORT_P5, GPIO_PIN6                     //������ĸ�������

void TimA2_Cap_Init(void)
{
    // 1.�������
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(CAP_PORT_PIN, GPIO_PRIMARY_MODULE_FUNCTION);

    /* ��ʱ�����ò���*/
    Timer_A_ContinuousModeConfig continuousModeConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,      // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_48, // SMCLK/48 = 1MHz
        TIMER_A_TAIE_INTERRUPT_ENABLE,  // ������ʱ������ж�
        TIMER_A_DO_CLEAR                // Clear Counter
    };
    // 3.����ʱ����ʼ��Ϊ��������ģʽ
    MAP_Timer_A_configureContinuousMode(CAP_TIMA_SELECTION, &continuousModeConfig);

    // 4.���ò�׽ģʽ�ṹ�� */
    const Timer_A_CaptureModeConfig captureModeConfig_TA2 = {
        CAP_REGISTER_SELECTION,                      //�����������
        TIMER_A_CAPTUREMODE_RISING_AND_FALLING_EDGE, //�����½��ز���
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,           //CCIxA:�ⲿ��������  ��CCIxB:���ڲ�ACLK����(�ֲ�)
        TIMER_A_CAPTURE_SYNCHRONOUS,                 //ͬ������
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,     //����CCRN�����ж�
        TIMER_A_OUTPUTMODE_OUTBITVALUE               //���λֵ
    };
    // 5.��ʼ����ʱ���Ĳ���ģʽ
    MAP_Timer_A_initCapture(CAP_TIMA_SELECTION, &captureModeConfig_TA2);

    // 6.ѡ������ģʽ������ʼ����
    MAP_Timer_A_startCounter(CAP_TIMA_SELECTION, TIMER_A_CONTINUOUS_MODE);

    // 7.����жϱ�־λ
    MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION);                                   //�����ʱ������жϱ�־λ
    MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION); //��� CCR1 �����жϱ�־λ

    // 8.������ʱ���˿��ж�
    MAP_Interrupt_enableInterrupt(INT_TA2_N); //������ʱ��A2�˿��ж�
}
// 10.��дTIMA ISR ��������

// TIMA2_CAP_STA ����״̬
// [7]:����ߵ�ƽ���״̬
// [6]:0��ʾδ���������أ�1��ʾ�����������
// [5:0]:�������
uint8_t TIMA2_CAP_STA = 0;
uint16_t TIMA2_CAP_VAL = 0;

void TA2_N_IRQHandler(void)
{
    if ((TIMA2_CAP_STA & 0X80) == 0) //��δ�ɹ�����
    {
        if (MAP_Timer_A_getEnabledInterruptStatus(CAP_TIMA_SELECTION)) //����ж�
        {
            MAP_Timer_A_clearInterruptFlag(CAP_TIMA_SELECTION); //�����ʱ������жϱ�־λ

            /* �� �����λCOV �� */
            /* ����UP���ǽ��ˣ������δ����ж�λֵʱ������һ���жϣ�COV����λ����Ҫ�����λ������û�йٷ��⺯����������Բο������ֲ�(slau356h.pdf) P790 */
            BITBAND_PERI(TIMER_A_CMSIS(CAP_TIMA_SELECTION)->CCTL[CAP_CCR_NUM], TIMER_A_CCTLN_COV_OFS) = 0;

            if (TIMA2_CAP_STA & 0X40) //�Ѿ����񵽸ߵ�ƽ�� 40H = 0x01000000
            {
                if ((TIMA2_CAP_STA & 0X3F) == 0X3F) //�ߵ�ƽ̫����
                {
                    TIMA2_CAP_STA |= 0X80; //ǿ�Ʊ�ǳɹ�������ߵ�ƽ 80H = 0x10000000
                    TIMA2_CAP_VAL = 0XFFFF;
                }
                else
                    TIMA2_CAP_STA++; //���������1
            }
        }

        if (MAP_Timer_A_getCaptureCompareEnabledInterruptStatus(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION)) //�����ж�
        {
            MAP_Timer_A_clearCaptureCompareInterrupt(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION); //��� CCR1 �����жϱ�־λ

            //�ж��Ƿ񲶻��½���
            if (TIMA2_CAP_STA & 0X40 && (MAP_Timer_A_getSynchronizedCaptureCompareInput(CAP_TIMA_SELECTION,
                                                                                        CAP_REGISTER_SELECTION,
                                                                                        TIMER_A_READ_CAPTURE_COMPARE_INPUT) == TIMER_A_CAPTURECOMPARE_INPUT_LOW))
            {
                TIMA2_CAP_STA |= 0X80; //��ǳɹ�������ߵ�ƽ
                TIMA2_CAP_VAL = Timer_A_getCaptureCompareCount(CAP_TIMA_SELECTION, CAP_REGISTER_SELECTION);
            }
            else //��δ��ʼ,��һ�β���������
            {
                TIMA2_CAP_STA = 0;
                TIMA2_CAP_VAL = 0;
                MAP_Timer_A_clearTimer(CAP_TIMA_SELECTION); //��ն�ʱ�� ���´�0����
                TIMA2_CAP_STA |= 0X40;                      //��ǲ�����������
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
