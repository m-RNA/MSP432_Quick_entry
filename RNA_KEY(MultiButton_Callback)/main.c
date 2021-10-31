/****************************************************/
//MSP432P401R
//2-2* GPIO����(MultiButton��)
//��Ŀ��ַ https://github.com/0x1abin/MultiButton
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/10/29
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "MultiTimer.h"
#include "key.h"
#include "led.h"

// ������key.c���� 
// ��������¼�����Ӻ󣬵�����˫������������
// �۲촮�ڴ�ӡ�ص�ֵ

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
