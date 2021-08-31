#include "sysinit.h"
#include "uartinit.h"
#include "delay.h"
#include "led.h"

/******************************************************************************
//MSP432P401R
//4 ��ֲ�δ���ʱ
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/16
*******************************************************************************/

int main(void)
{
  SysInit();    //��3�� ʱ������
  UART_Init();  //��7�� ����ʵ��
  delay_init(); //��4�� �δ���ʱ
  LED_Init();   //��2�� GPIO���

  printf("MSP432\r\n");
  printf("2021/8/16\r\n");

  while (1)
  {
    LED_RED_Tog();
    delay_ms(250);
  }
}
