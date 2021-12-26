/****************************************************/
//MSP432P401R
//LED驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/10
/****************************************************/

/**************************
  *
  * V1.2 2021/12/27
  * 支持位带操作以提高效率
  * 
  * V1.1 2021/9/13 
  * 添加其他颜色 
  * 并为了支持Multi_timer扩展包
  * 将宏定义修改为函数
  *
  * V1.0 2021/8/10
  * 完成基本驱动 
  *
  ************************/

#ifndef __LED_H
#define __LED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

// 位带操作
#define LED_RED BITBAND_PERI(P1OUT,0)
#define LED_R BITBAND_PERI(P2OUT,0)
#define LED_G BITBAND_PERI(P2OUT,1)
#define LED_B BITBAND_PERI(P2OUT,2)

void LED_Init(void);

void LED_RED_On(void);
void LED_RED_Off(void);
void LED_RED_Tog(void);

void LED_Y_On(void);
void LED_C_On(void);
void LED_P_On(void);

void LED_R_On(void);
void LED_G_On(void);
void LED_B_On(void);

void LED_R_Off(void);
void LED_G_Off(void);
void LED_B_Off(void);

void LED_R_Tog(void);
void LED_G_Tog(void);
void LED_B_Tog(void);

void LED_W_On(void);
void LED_W_Off(void);
void LED_W_Tog(void);

#endif
