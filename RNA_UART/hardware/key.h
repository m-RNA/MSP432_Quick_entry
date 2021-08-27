#ifndef __KEY_H
#define __KEY_H
#include "sysinit.h"
/****************************************************/
//MSP432P401R
//���ذ�������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
/****************************************************/

#define KEY1 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //��ȡ����1
#define KEY2 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //��ȡ����2

#define KEY1_PRES 1 //KEY1����
#define KEY2_PRES 2 //KEY2����

void KEY_Init(bool mode);    //IO��ʼ��
uint8_t KEY_Scan(bool mode); //����ɨ�躯��

#endif
