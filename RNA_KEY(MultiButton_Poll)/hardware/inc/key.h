/****************************************************/
//MSP432P401R
//���ذ�������(MultiButton��)
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/10/29
/****************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "multi_button.h"

#define KEY1 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //��ȡ����1
#define KEY2 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //��ȡ����2

extern struct Button btn1_OnBoard;
extern struct Button btn2_OnBoard;

void KEY_Init(void);    //IO��ʼ��

#endif
