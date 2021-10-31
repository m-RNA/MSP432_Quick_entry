/****************************************************/
//MSP432P401R
//板载按键驱动(MultiButton版)
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/10/29
/****************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "multi_button.h"

#define KEY1 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //读取按键1
#define KEY2 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //读取按键2

extern struct Button btn1_OnBoard;
extern struct Button btn2_OnBoard;

void KEY_Init(void);    //IO初始化

#endif
