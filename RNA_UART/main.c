/******************************************************************************
// MSP432P401R
// 7 ��������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/25
*******************************************************************************/

/*
 * 2021/10/18 ����
 * 
 * CCS֧��printf
 * Keil֧�ֱ�׼C���΢��
 * ��Keil�������ڿ��Բ���΢����
 *
 * ʹ�ñ�׼C��ʱ�����޷�ʹ��scanf��
 * �����Ҫʹ��scanfʱ����ʹ��΢�� MicroLIB��
 * 
 */

#include "sysinit.h"
#include "usart.h"

int main(void)
{
	SysInit();		   //��3�� ʱ������
	uart_init(115200); //��7�� ����ʵ��
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
		// ʹ��΢�����֧�� scanf
		//char a[100];
		//scanf("%s", a);
		//printf("%s\r\n", a);
	}
}
