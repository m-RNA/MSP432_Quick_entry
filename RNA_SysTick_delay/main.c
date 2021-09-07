#include "sysinit.h"
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
  delay_init(); //��4�� �δ���ʱ
  LED_Init();   //��2�� GPIO���

  while (1)
  {
    LED_RED_Tog();
    delay_ms(250);
  }
}
