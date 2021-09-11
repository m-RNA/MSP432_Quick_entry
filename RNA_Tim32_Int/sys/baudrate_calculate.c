#include "baudrate_calculate.h"

/****************************************************/
// MSP432P401R
// 串口波特率计算
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************************    说明    ******************************
 *
 * 源码为TI官方编写,本人只是将JS程序移植到了C语言平台,仅作为学习使用。源码出处为：
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 *
 * ? 已知问题：
 * 调试时发现某些情况下，C语言的小数的大小与JS的相差较大，
 * 导致了算出的UCSx(即secondModReg)不一样，
 * 这时如果出现不能准确传输时，请换一个波特率。
 *
 * ? 需要注意：
 * 波特率不能大于时钟频率，否则会退出函数
 *
 * *****************************   版本说明   ******************************
 *
 * ? v1.2 2021/8/29
 * 注释掉了闪烁灯的代码
 * 
 * ? v1.1  2021/8/27
 * 添加支持固件库v3_21_00_05
 *
 * ? v1.0  2021/8/25
 * 仅支持固件库v3_40_01_02
 *
 * *******************************   结束    *******************************/

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
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uart_config, uint32_t baudRate) //固件库v3_40_01_02
#else
void eusci_calcBaudDividers(eUSCI_UART_Config *uart_config, uint32_t baudRate) //固件库v3_21_00_05
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

    if (!uart_config || !baudRate) //传参错误 退出函数
    {
        //uart_warning_led(); //闪烁错误指示灯10次
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
    if (baudRate > clockRate) //判断波特率是否大于时钟频率 是则退出函数
    {
        //uart_warning_led(); //闪烁错误指示灯10次
        return;
    }
    //var result = {UCOS16 : 0, UCBRx : 0, UCFx : 0, UCSx : 0, maxAbsError : 0};

    NN = (uint16_t)((float)clockRate / (float)baudRate); //应该是不需要floor

    minAbsError = 100000;
    for (jj = 0; jj <= 255; jj++)
    {

        maxAbsErrorInByte = 0;
        count = 0;
        for (ii = 0; ii <= 10; ii++)
        {
            count += NN + bitPosition(jj, 7 - (ii % 8));

            //error = (ii + 1) * baudPeriod - count * clockPeriod;
            error = (ii + 1) / (float)baudRate - count / (float)clockRate; //为了减少变量，改为此代码

            if (error < 0)
                error = -error;

            if (error > maxAbsErrorInByte)
                maxAbsErrorInByte = error;
        }
        if (maxAbsErrorInByte - minAbsError < -7.3e-12f) //这里就是“已知问题”
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
        uart_config->clockPrescalar = (uint16_t)((float)NN / 16.0f); //应该是不需要floor
        uart_config->firstModReg = NN - (uart_config->clockPrescalar * 16);
    }
    //return minAbsError * baudRate * 100;
}

////闪烁错误指示灯10次
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
