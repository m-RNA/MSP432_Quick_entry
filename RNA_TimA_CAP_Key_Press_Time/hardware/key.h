#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/**************************************************************************************/
/*************                   ����ԭ��@ALIENTEK                **********************/
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
/* ˵�� ��
 * Դ��Ϊ����ԭ��@ALIENTEK ��д
 * ����RNA��ֻ����ֲ����MSP432ƽ̨
 * ����Ϊѧϰʹ��
 * �ڴ�Ҳ��л����ԭ��
 */
/**************************************************************************************/

/******************************************************************************
//MSP432P401R
//2-2 GPIO���� ���ذ���
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/12
*******************************************************************************/

#define KEY1 GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //��ȡ����1
#define KEY2 GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //��ȡ����2

#define KEY1_PRES 1 //KEY1����
#define KEY2_PRES 2 //KEY2����

void KEY_Init(void);       //IO��ʼ��
uint8_t KEY_Scan(uint8_t); //����ɨ�躯��
#endif
