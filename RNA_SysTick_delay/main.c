#include "sysinit.h"
#include "delay.h"
#include "led.h"

/******************************************************************************
//MSP432P401R
//4 移植滴答延时
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/16
*******************************************************************************/

int main(void)
{
  SysInit();    //第3讲 时钟配置
  delay_init(); //第4讲 滴答延时
  LED_Init();   //第2讲 GPIO输出

  while (1)
  {
    LED_RED_Tog();
    delay_ms(250);
  }
}
