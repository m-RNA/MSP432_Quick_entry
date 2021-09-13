#include "sysinit.h"
#include "uartinit.h"
#include "delay.h"
#include "adc.h"

/******************************************************************************
//MSP432P401R
//11 ADC ��ͨ���ɼ�
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/9/13
*******************************************************************************/

int main(void)
{
    SysInit();         // ��3�� ʱ������
    uart_init(115200); // ��7�� ��������
    delay_init();      // ��4�� �δ���ʱ
	
	ADC_Config(); // ��11�� ADC�ɼ� 
	
    printf("Hello MSP432!\r\n");
	
    while (1)
    {
    }
}

