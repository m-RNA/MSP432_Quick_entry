
/**************************************************************************************/
/*************                   正点原子@ALIENTEK                **********************/
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
/* 说明 ：
 * 源码为正点原子@ALIENTEK 编写
 * 本人RNA，只是移植到了MSP432平台
 * 仅作为学习使用
 * 在此也感谢正点原子
 */
/**************************************************************************************/

/******************************************************************************
//MSP432P401R
//2-2 GPIO输入 板载按键
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/12
*******************************************************************************/

#ifndef __KEY_H
#define __KEY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define KEY1 GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) //读取按键1
#define KEY2 GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) //读取按键2

#define KEY1_PRES 1 //KEY1按下
#define KEY2_PRES 2 //KEY2按下

void KEY_Init(void);       //IO初始化
uint8_t KEY_Scan(uint8_t); //按键扫描函数
#endif
