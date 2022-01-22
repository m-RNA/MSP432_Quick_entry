/****************************************************/
// MSP432P401R
// 定时器A
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

#ifndef __RNA_TIMA_H
#define __RNA_TIMA_H
#include "sysinit.h"

//--------------------------  定时器相关函数   -----------------------------------//
void TimA1_PWM_Init(uint16_t ccr0, uint16_t psc);
void TimA0_Int_Init(uint16_t ccr0, uint16_t psc);

//--------------------------SPWM波相关开始----------------------------------------//
#define SamplesPerCycle 264                        //半周期占空比个数
#define Tim1Period (48000000 / (CarrierFrequency)) //此值不能超过65536
#define Tim1Prescaler 1
#define Tim0Period (48000000 / (SamplesPerCycle * FundamentalFrequency))
#define Tim0Prescaler 1

extern double MaxDutyCycle;           //最大占空比比例系数
extern uint16_t CarrierFrequency;     //SPWM波 载波频率，单位HZ
extern uint16_t FundamentalFrequency; //SPWM波基波频率，单位HZ

void SpwmInit(void);
//--------------------------SPWM波相关结束------------------------------------ ---//

#endif
