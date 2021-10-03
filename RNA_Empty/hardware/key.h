#ifndef __KEY_H
#define __KEY_H
#include "sysinit.h"
/****************************************************/
//MSP432P401R
//板载按键驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

#define KEY1 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //读取按键1
#define KEY2 MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //读取按键2

#define KEY1_PRES 1 //KEY1按下
#define KEY2_PRES 2 //KEY2按下

void KEY_Init(bool mode);    //IO初始化
uint8_t KEY_Scan(bool mode); //按键扫描函数

#endif
