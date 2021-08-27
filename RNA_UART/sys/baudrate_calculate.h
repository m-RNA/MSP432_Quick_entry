#ifndef __RNA_BAUDRATE_CALCULATE_H
#define __RNA_BAUDRATE_CALCULATE_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/****************************************************/
// MSP432P401R
// ���ڲ����ʼ���
// �汾 v1.0
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/25
/****************************************************/

/* ************   ˵��   ************
 *
 * Դ��ΪTI�ٷ���д,����ֻ�ǽ�JS����
 * ��ֲ����C����ƽ̨,����Ϊѧϰʹ�á�
 * Դ�����Ϊ��http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * 
 * v1.0 ��֪���⣺
 * ����ʱ����C���Ե�С���Ĵ�С��ٷ�JS�Ĳ�һ����
 * �����������UCSx(��secondModReg)��һ����
 * ��ʱ������ֲ���׼ȷ����ʱ���뻻һ�������ʡ�
 *
 * ************   ����    ************/

//����ָʾ�ƺ궨�� ������ֲʹ��
//MSP432P401R ���������P1.0 P2.0
#define WARN_LED_1_PORT GPIO_PORT_P1
#define WARN_LED_1_PIN GPIO_PIN0
#define WARN_LED_2_PORT GPIO_PORT_P2
#define WARN_LED_2_PIN GPIO_PIN0
#define WARN_LED_1_INIT() MAP_GPIO_setAsOutputPin(WARN_LED_1_PORT, WARN_LED_1_PIN)
#define WARN_LED_2_INIT() MAP_GPIO_setAsOutputPin(WARN_LED_2_PORT, WARN_LED_2_PIN)
#define WARN_LED_1_ON() MAP_GPIO_setOutputHighOnPin(WARN_LED_1_PORT, WARN_LED_1_PIN);
#define WARN_LED_1_OFF() MAP_GPIO_setOutputLowOnPin(WARN_LED_1_PORT, WARN_LED_1_PIN);
#define WARN_LED_2_ON() MAP_GPIO_setOutputHighOnPin(WARN_LED_2_PORT, WARN_LED_2_PIN);
#define WARN_LED_2_OFF() MAP_GPIO_setOutputLowOnPin(WARN_LED_2_PORT, WARN_LED_2_PIN);

/******************   �汾˵��   *****************
 * 
 * ? v1.0  2021/8/25
 * 
 * ? ��Ҫע�⣺
 * �����ʲ��ܴ���ʱ��Ƶ�ʣ�������򽫻�ת����˸����ָʾ�Ƶ���ѭ����
 * 
 * 
 ****************************************************/
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uartConfig, uint32_t baudRate);

#endif
