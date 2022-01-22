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
  
#include "led.h"

void LED_Init(void)
{
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2);

    LED_RED_Off();
    LED_R_Off();
    LED_G_Off();
    LED_B_Off();
}
void LED_RED_On(void) { LED_RED = 1; }
void LED_RED_Off(void) { LED_RED = 0; }
void LED_RED_Tog(void) { LED_RED ^= 1; }

void LED_R_Off(void) { LED_R = 0;}
void LED_G_Off(void) { LED_G = 0;}
void LED_B_Off(void) { LED_B = 0; }

void LED_R_On(void) { LED_R = 1; }
void LED_G_On(void) { LED_G = 1;  }
void LED_B_On(void) { LED_B = 1;  }

void LED_R_Tog(void) { LED_R ^= 1; }
void LED_G_Tog(void) { LED_G ^= 1; }
void LED_B_Tog(void) { LED_B ^= 1; }

//白色 White
void LED_W_On(void)
{
    LED_R_On();
    LED_G_On();
    LED_B_On();
}
//白色 White
void LED_W_Off(void)
{
    LED_R_Off();
    LED_G_Off();
    LED_B_Off();
}
//白色 White
void LED_W_Tog(void)
{
    LED_R_Tog();
    LED_G_Tog();
    LED_B_Tog();
}
//黄色 Yellow
void LED_Y_On(void)
{
    LED_R_On();
    LED_G_On();
    LED_B_Off();
}
//品红 Pinkish red
void LED_P_On(void)
{
    LED_R_On();
    LED_G_Off();
    LED_B_On();
}
//青色 Cyan
void LED_C_On(void)
{
    LED_R_Off();
    LED_G_On();
    LED_B_On();
}
