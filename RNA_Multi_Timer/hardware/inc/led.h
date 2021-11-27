/****************************************************/
//MSP432P401R
//LED驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/10
/****************************************************/

/**************************
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
