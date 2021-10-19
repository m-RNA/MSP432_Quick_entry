/******************************************************************************
// MSP432P401R
// 7 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/25
*******************************************************************************/

/*
 * 2021/10/18 更新
 * 
 * CCS支持printf
 * Keil支持标准C库跟微库
 * 用Keil开发终于可以不开微库啦
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
	printf("2021/8/24\r\n\r\n");

	char c = '!';
	char *s = "printf test";
	int i = -12345;
	unsigned u = 4321;
	long int l = -123456780;
	unsigned long n = 1098765432;
	unsigned x = 0x89AB;

	printf("Char           %c\r\n", c);
	printf("String         %s\r\n", s);
	printf("Integer        %d\r\n", i);
	printf("Unsigned       %u\r\n", u);
	printf("Long           %d\r\n", l);
	printf("Unsigned long  %u\r\n", n);
	printf("HEX            %X\r\n", x);

	while (1)
	{
		// 使用微库则可支持 scanf
		//char a[100];
		//scanf("%s", a);
		//printf("%s\r\n", a);
	}
}
