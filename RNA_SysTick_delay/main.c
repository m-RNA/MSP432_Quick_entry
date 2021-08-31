#include "sysinit.h"
#include "uartinit.h"
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
  UART_Init();  //第7讲 串口实验
  delay_init(); //第4讲 滴答延时
  LED_Init();   //第2讲 GPIO输出

  printf("MSP432\r\n");
  printf("2021/8/16\r\n");

  while (1)
  {
    LED_RED_Tog();
    delay_ms(250);
  }
}
