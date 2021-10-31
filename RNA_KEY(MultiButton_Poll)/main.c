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
	static uint8_t btn1_OnBoard_event_val;
	static uint8_t btn2_OnBoard_event_val;

	SysInit();		   // ��3�� ʱ������
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

		btn1_OnBoard_event_val = get_button_event(&btn1_OnBoard);
		btn2_OnBoard_event_val = get_button_event(&btn2_OnBoard);

		switch (btn1_OnBoard_event_val)
		{
		case PRESS_DOWN:
			//printf("btn1_OnBoard PRESS_DOWN\r\n");
			break;
		case PRESS_UP:
			//printf("btn1_OnBoard PRESS_UP\r\n");
			break;
		case PRESS_REPEAT:
			//printf("btn1_OnBoard PRESS_REPEAT\r\n");
			break;
		case SINGLE_CLICK:
			printf("btn1_OnBoard SINGLE_CLICK\r\n");
			break;
		case DOUBLE_CLICK:
			printf("btn1_OnBoard DOUBLE_CLICK\r\n");
			break;
		case LONG_PRESS_START:
			//printf("btn1_OnBoard LONG_PRESS_START\r\n");
			break;
		case LONG_PRESS_HOLD:
			printf("btn1_OnBoard LONG_PRESS_HOLD\r\n");
			break;
		}

		switch (btn2_OnBoard_event_val)
		{
		case PRESS_DOWN:
			//printf("btn2_OnBoard PRESS_DOWN\r\n");
			break;
		case PRESS_UP:
			//printf("btn2_OnBoard PRESS_UP\r\n");
			break;
		case PRESS_REPEAT:
			//printf("btn2_OnBoard PRESS_REPEAT\r\n");
			break;
		case SINGLE_CLICK:
			printf("btn2_OnBoard SINGLE_CLICK\r\n");
			break;
		case DOUBLE_CLICK:
			printf("btn2_OnBoard DOUBLE_CLICK\r\n");
			break;
		case LONG_PRESS_START:
			//printf("btn2_OnBoard LONG_PRESS_START\r\n");
			break;
		case LONG_PRESS_HOLD:
			printf("btn2_OnBoard LONG_PRESS_HOLD\r\n");
			break;
		}

		btn1_OnBoard_event_val = NONE_PRESS;
		btn2_OnBoard_event_val = NONE_PRESS;
		delay_ms(10);
		/*ֹͣ����û�����*/
	}
}
