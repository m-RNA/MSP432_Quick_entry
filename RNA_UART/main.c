/******************************************************************************
// MSP432P401R
// 7 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/25
*******************************************************************************/

/*
 * 2021/10/15 更新
 * 
 * 此版本支持 标准C库
 * 终于可以不使用微库啦
 *
 * 使用标准C库时，将无法使用scanf；
 * 如果需要使用scanf时，请使用微库 MicroLIB。
 * 
 */

#include "sysinit.h"
#include "usart.h"

int main(void)
{
	SysInit();		   //第3讲 时钟配置
	uart_init(115200); //第7讲 串口实验

	printf("MSP432\r\n");
	printf("2021/8/24\r\n");

	while (1)
	{
		// 使用微库则可支持 scanf
		//char a[100];
		//scanf("%s", a);
        //printf("%s\r\n", a);
	}
}
