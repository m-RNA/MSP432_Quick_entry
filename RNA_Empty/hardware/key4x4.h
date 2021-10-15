/****************************************************/
//MSP432P401R
//外接矩阵键盘驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

#ifndef __KEY4X4_H
#define __KEY4X4_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void KEY4x4_Init(void);
uint8_t KEY4x4_Scan(bool mode);

#endif
