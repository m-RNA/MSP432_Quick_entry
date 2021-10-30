/****************************************************/
// MSP432P401R
// ����Keil�����չ��� (�ѽ�ti�̼�����������)
// Keil���������ÿ���FPU
// ��������� MultiTimer��MultiButton
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/10/28
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "MultiTimer.h"
#include "key.h"
#include "led.h"

int main(void)
{
    SysInit();         // ��3�� ʱ������
    uart_init(115200); // ��7�� ��������
    MultiTimerInit();  // ��4*�� �����ʱ��+�δ���ʱ
	KEY_Init();		   // ��2�� GPIO����(MultiButton)
	LED_Init();		   // ��2�� GPIO���
	
    /*��ʼ����ʼ������*/

    /*ֹͣ����ʼ������*/

    printf("Hello,MSP432!\r\n");

    MAP_Interrupt_enableMaster(); // �������ж�
    while (1)
    {
		MultiTimerYield(); //��������ѭ�����ö�ʱ����̨������
		
        /*��ʼ����û�����*/

        /*ֹͣ����û�����*/
    }
}
