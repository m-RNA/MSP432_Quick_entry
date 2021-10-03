#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "tim32.h"
#include "multi_timer.h"

/****************************************************/
// MSP432P401R
// 配置Keil独立空工程 (已将ti固件库打包至工程)
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/28
/****************************************************/

int main(void)
{
	SysInit();		   // 第3讲 时钟配置
	uart_init(115200); // 第7讲 串口配置
	delay_init();	   // 第4讲 滴答延时

	/*开始填充初始化代码*/

	/*停止填充初始化代码*/

	printf("Hello,World!\r\n");
	printf("加油，电赛人！\r\n");

	MAP_Interrupt_enableMaster(); // 开启总中断

	while (1)
	{
		/*开始填充用户代码*/

		/*停止填充用户代码*/
	}
}
