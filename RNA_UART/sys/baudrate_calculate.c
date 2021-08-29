#include "baudrate_calculate.h"

/****************************************************/
// MSP432P401R
// ���ڲ����ʼ���
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************************    ˵��    ******************************
 *
 * Դ��ΪTI�ٷ���д,����ֻ�ǽ�JS������ֲ����C����ƽ̨,����Ϊѧϰʹ�á�Դ�����Ϊ��
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *
 * ? ��֪���⣺
 * ����ʱ����ĳЩ����£�C���Ե�С���Ĵ�С��JS�����ϴ�
 * �����������UCSx(��secondModReg)��һ����
 * ��ʱ������ֲ���׼ȷ����ʱ���뻻һ�������ʡ�
 *
 * ? ��Ҫע�⣺
 * �����ʲ��ܴ���ʱ��Ƶ�ʣ�������˳�����
 *
 * *****************************   �汾˵��   ******************************
 *
 * ? v1.2 2021/8/29
 * ע�͵�����˸�ƵĴ���
 * 
 * ? v1.1  2021/8/27
 * ���֧�ֹ̼���v3_21_00_05
 *
 * ? v1.0  2021/8/25
 * ��֧�ֹ̼���v3_40_01_02
 *
 * *******************************   ����    *******************************/

void uart_warning_led(void);

/*
 *  ======== bitPosition ========
 *  return 1(0) if the specified bit position in value is set(clear)
 */
bool bitPosition(uint16_t value, uint16_t position)
{
    if ((value & (1 << position)))
        return 1;
    return 0;
}

/*
 *  ======== eusci_calcBaudDividers ========
 *  computes the eUSCI_UART register settings for a given clock and baud rate
 *
 *      UCOS16:      the oversampling bit (0 or 1)
 *      UCBRx:       the Baud Rate Control Word
 *      UCFx:        the First modulation stage select (UCBRFx)
 *      UCSx:        the Second modulation stage select (UCBRSx)
 *      maxAbsError: the maximum TX error for the register setting above
 *
 *  The first four field names match the names used in Table 18-5,
 *  "Recommended Settings for Typical Crystals and Baudrates", of the
 *  MSP430FR57xx Family User's Guide (SLAU272A).
 */
#ifdef EUSCI_A_UART_7_BIT_LEN
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uart_config, uint32_t baudRate) //�̼���v3_40_01_02
#else
void eusci_calcBaudDividers(eUSCI_UART_Config *uart_config, uint32_t baudRate) //�̼���v3_21_00_05
#endif
{
    float maxAbsErrorInByte;
    float minAbsError;
    float error;
    uint8_t ii;
    uint16_t jj;
    uint16_t NN;
    uint32_t count;
    uint32_t clockRate;

    if (!uart_config || !baudRate) //���δ��� �˳�����
    {
        //uart_warning_led(); //��˸����ָʾ��10��
        return;
    }

    if (uart_config->selectClockSource == EUSCI_A_UART_CLOCKSOURCE_SMCLK)
        clockRate = MAP_CS_getSMCLK();
    else if (uart_config->selectClockSource == EUSCI_A_UART_CLOCKSOURCE_ACLK)
        clockRate = MAP_CS_getACLK();
    else
    {
        uart_config->selectClockSource = EUSCI_A_UART_CLOCKSOURCE_SMCLK;
        clockRate = MAP_CS_getSMCLK();
    }
    if (baudRate > clockRate) //�жϲ������Ƿ����ʱ��Ƶ�� �����˳�����
    {
        //uart_warning_led(); //��˸����ָʾ��10��
        return;
    }
    //var result = {UCOS16 : 0, UCBRx : 0, UCFx : 0, UCSx : 0, maxAbsError : 0};

    NN = (uint16_t)((float)clockRate / (float)baudRate); //Ӧ���ǲ���Ҫfloor

    minAbsError = 100000;
    for (jj = 0; jj <= 255; jj++)
    {

        maxAbsErrorInByte = 0;
        count = 0;
        for (ii = 0; ii <= 10; ii++)
        {
            count += NN + bitPosition(jj, 7 - (ii % 8));

            //error = (ii + 1) * baudPeriod - count * clockPeriod;
            error = (ii + 1) / (float)baudRate - count / (float)clockRate; //Ϊ�˼��ٱ�������Ϊ�˴���

            if (error < 0)
                error = -error;

            if (error > maxAbsErrorInByte)
                maxAbsErrorInByte = error;
        }
        if (maxAbsErrorInByte - minAbsError < -7.3e-12f) //������ǡ���֪���⡱
        {
            minAbsError = maxAbsErrorInByte;
            uart_config->secondModReg = jj;
        }
    }

    if (NN < 20)
    {
        uart_config->overSampling = 0;
        uart_config->clockPrescalar = NN;
        uart_config->firstModReg = 0;
    }
    else
    {
        uart_config->overSampling = 1;
        uart_config->clockPrescalar = (uint16_t)((float)NN / 16.0f); //Ӧ���ǲ���Ҫfloor
        uart_config->firstModReg = NN - (uart_config->clockPrescalar * 16);
    }
    //return minAbsError * baudRate * 100;
}

////��˸����ָʾ��10��
//void uart_warning_led(void)
//{
//    uint8_t ii;
//    uint32_t jj;
//    WARN_LED_INIT(WARN_LED_1_PORT, WARN_LED_1_PIN);
//    WARN_LED_INIT(WARN_LED_2_PORT, WARN_LED_2_PIN);
//    for (ii = 0; ii < 10; ii++)
//    {
//        WARN_LED_ON(WARN_LED_1_PORT, WARN_LED_1_PIN);
//        WARN_LED_OFF(WARN_LED_2_PORT, WARN_LED_2_PIN);
//        for (jj = 0; jj < 100000; jj++)
//            ;
//        WARN_LED_OFF(WARN_LED_1_PORT, WARN_LED_1_PIN);
//        WARN_LED_ON(WARN_LED_2_PORT, WARN_LED_2_PIN);
//        for (jj = 0; jj < 100000; jj++)
//            ;
//    }
//}
