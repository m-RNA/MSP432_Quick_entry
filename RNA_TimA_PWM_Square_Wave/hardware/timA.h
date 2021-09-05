#ifndef __RNA_TIMA_H
#define __RNA_TIMA_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

void TimA0_Int_Init(uint16_t ccr0, uint16_t psc);

#endif
