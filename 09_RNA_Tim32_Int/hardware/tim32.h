/****************************************************/
// MSP432P401R
// 定时器32
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/8
/****************************************************/

#ifndef __RNA_TIM32_H
#define __RNA_TIM32_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void Tim32_0_Int_Init(uint32_t aar, uint8_t psc);
void Tim32_1_Int_Init(uint32_t aar, uint8_t psc);

#endif
