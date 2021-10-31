/****************************************************/
// MSP432P401R
// 配置Keil独立空工程 (已将ti固件库打包至工程)
// Keil工程已配置开启FPU
// 加入软件包 MultiTimer、MultiButton
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/10/28
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

	SysInit();		   // 第3讲 时钟配置
	uart_init(115200); // 第7讲 串口配置
	MultiTimerInit();  // 第4*讲 软件定时器+滴答延时
	KEY_Init();		   // 第2讲 GPIO输入(MultiButton)
	LED_Init();		   // 第2讲 GPIO输出

	/*开始填充初始化代码*/

	/*停止填充初始化代码*/

	printf("Hello,MSP432!\r\n");
	MAP_Interrupt_enableMaster(); // 开启总中断
	while (1)
	{
		MultiTimerYield(); //必须在主循环调用定时器后台处理函数

		/*开始填充用户代码*/

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
		/*停止填充用户代码*/
	}
}
