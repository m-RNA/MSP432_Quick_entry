#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "tim32.h"
#include "multi_timer.h"

/****************************************************/
// MSP432P401R
// ����Keil�����չ��� (�ѽ�ti�̼�����������)
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/28
/****************************************************/

int main(void)
{
	SysInit();		   // ��3�� ʱ������
	uart_init(115200); // ��7�� ��������
	delay_init();	   // ��4�� �δ���ʱ

	/*��ʼ����ʼ������*/

	/*ֹͣ����ʼ������*/

	printf("Hello,World!\r\n");
	printf("���ͣ������ˣ�\r\n");

	MAP_Interrupt_enableMaster(); // �������ж�

	while (1)
	{
		/*��ʼ����û�����*/

		/*ֹͣ����û�����*/
	}
}
