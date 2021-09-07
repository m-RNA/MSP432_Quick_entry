#ifndef __RNA_BAUDRATE_CALCULATE_H
#define __RNA_BAUDRATE_CALCULATE_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

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
 * ����ʱ����C���Ե�С���Ĵ�С��ٷ�JS�Ĳ�һ����
 * �����������UCSx(��secondModReg)��һ����
 * ��ʱ������ֲ���׼ȷ����ʱ���뻻һ�������ʡ�
 *
 * ? ��Ҫע�⣺
 * �����ʲ��ܴ���ʱ��Ƶ�ʣ�������˳�����
 * 
 * *****************************   �汾˵��   ******************************
 * 
 * ? v1.1  2021/8/27
 * ���֧�ֹ̼���v3_21_00_05 
 * 
 * ? v1.0  2021/8/25
 * ��֧�ֹ̼���v3_40_01_02
 * 
 * *******************************   ����    *******************************/

//����ָʾ�ƺ궨�� ������ֲʹ��
//MSP432P401R ���������P1.0 P2.0
//#define WARN_LED_1_PORT GPIO_PORT_P1
//#define WARN_LED_2_PORT GPIO_PORT_P2
//#define WARN_LED_1_PIN GPIO_PIN0
//#define WARN_LED_2_PIN GPIO_PIN0
//#define WARN_LED_INIT MAP_GPIO_setAsOutputPin
//#define WARN_LED_ON MAP_GPIO_setOutputHighOnPin
//#define WARN_LED_OFF MAP_GPIO_setOutputLowOnPin


#ifdef EUSCI_A_UART_7_BIT_LEN
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uart_config, uint32_t baudRate); //�̼���v3_40_01_02
#else
void eusci_calcBaudDividers(eUSCI_UART_Config *uart_config, uint32_t baudRate); //�̼���v3_21_00_05
#endif

#endif
