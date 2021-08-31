#include "led.h"
/****************************************************/
//MSP432P401R
//LED驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/10
/****************************************************/

void LED_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2);

    LED_RED_Off();
    LED_R_Off();
    LED_G_Off();
    LED_B_Off();
}

void LED_W_On(void)
{
    LED_R_On();
    LED_G_On();
    LED_B_On();
}
void LED_W_Off(void)
{
    LED_R_Off();
    LED_G_Off();
    LED_B_Off();
}
void LED_W_Tog(void)
{
    LED_R_Tog();
    LED_G_Tog();
    LED_B_Tog();
}
