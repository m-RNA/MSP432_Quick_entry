#ifndef __LED_H
#define __LED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/****************************************************/
//MSP432P401R
//LED驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/10
/****************************************************/

#define LED_RED_Off() MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0)
#define LED_R_Off() MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0)
#define LED_G_Off() MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1)
#define LED_B_Off() MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2)

#define LED_RED_On() MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0)
#define LED_R_On() MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0)
#define LED_G_On() MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1)
#define LED_B_On() MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2)

#define LED_RED_Tog() MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0)
#define LED_R_Tog() MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0)
#define LED_G_Tog() MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1)
#define LED_B_Tog() MAP_GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2)

void LED_Init(void);

void LED_W_On(void);
void LED_W_Off(void);
void LED_W_Tog(void);

#endif
