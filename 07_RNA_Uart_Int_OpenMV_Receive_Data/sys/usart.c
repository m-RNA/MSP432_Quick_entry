/****************************************************/
// MSP432P401R
// 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************   版本更新说明   *****************
 *
 * 此版本支持 标准C库
 * 终于可以不使用微库啦
 *
 * ? 需要注意：
 * ①使用标准C库时，将无法使用scanf。
 * 如果需要使用scanf时，请使用微库 MicroLIB
 * ①低频时钟频率下，高波特率使得传输时误差过大,
 * 比如35768Hz下19200波特率,
 * 会使得传输出错，这时可以尝试降低波特率。
 * ②baudrate_calculate的问题请去文件内查看。
 *
 * **************************************************
 *
 * ? v3.0  2021/10/15
 * 此版本支持使用 标准C库
 * 文件正式改名为与正点原子同名的
 * usart.c 和 usart.h
 * 方便移植
 *
 * ? v2.1  2021/8/27
 * 添加支持固件库v3_21_00_05
 * 仅支持 MicroLIB 微库
 *
 * ? v2.0  2021/8/25
 * uart_init增添了波特率传入参数，可直接配置波特率。
 * 计算UART的代码单独打包为名为
 * baudrate_calculate的c文件和h文件
 * 仅支持 MicroLIB 微库
 *
 * ? v1.0 2021/7/17
 * 仅支持固件库v3_40_01_02
 * 配置了SMCLK 48MHz 波特率 115200的初始化代码，
 * 对接标准输入输出库，使其能使用printf、scanf函数
 * 仅支持 MicroLIB 微库
 *
 ****************************************************/

#include "usart.h"
#include "baudrate_calculate.h"
#include "stdarg.h"

//预编译
// if 1 使用标准C库 如果报错就使用微库
// if 0 使用微库 得去勾选魔术棒里的 Use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
	int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
#else
int fgetc(FILE *f)
{
	while (EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG !=
		   UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
		;
	return UART_receiveData(EUSCI_A0_BASE);
}
#endif

int fputc(int ch, FILE *f)
{
	UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
	return ch;
}

/*****************   函数说明   *****************
 *
 * 以上两条对接标准输入输出库的函数:
 * int fputc(int ch, FILE *f);
 * int fgetc(FILE *f);
 * 源码为BiliBili平台UP主 “CloudBoyStudio” 编写
 * 本人RNA，不是作者
 * 在此也表感谢
 *
 *****************   说明结束   *****************/

void uart0_init(uint32_t baudRate)
{
#ifdef EUSCI_A_UART_7_BIT_LEN
	//固件库v3_40_01_02
	//默认SMCLK 48MHz 比特率 115200
	const eUSCI_UART_ConfigV1 uartConfig =
		{
			EUSCI_A_UART_CLOCKSOURCE_SMCLK,				   // SMCLK Clock Source
			26,											   // BRDIV = 26
			0,											   // UCxBRF = 0
			111,										   // UCxBRS = 111
			EUSCI_A_UART_NO_PARITY,						   // No Parity
			EUSCI_A_UART_LSB_FIRST,						   // MSB First
			EUSCI_A_UART_ONE_STOP_BIT,					   // One stop bit
			EUSCI_A_UART_MODE,							   // UART mode
			EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
			EUSCI_A_UART_8_BIT_LEN						   // 8 bit data length
		};
	eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //配置波特率
#else
	//固件库v3_21_00_05
	//默认SMCLK 48MHz 比特率 115200
	const eUSCI_UART_Config uartConfig =
		{
			EUSCI_A_UART_CLOCKSOURCE_SMCLK,				   // SMCLK Clock Source
			26,											   // BRDIV = 26
			0,											   // UCxBRF = 0
			111,										   // UCxBRS = 111
			EUSCI_A_UART_NO_PARITY,						   // No Parity
			EUSCI_A_UART_LSB_FIRST,						   // MSB First
			EUSCI_A_UART_ONE_STOP_BIT,					   // One stop bit
			EUSCI_A_UART_MODE,							   // UART mode
			EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
		};
	eusci_calcBaudDividers((eUSCI_UART_Config *)&uartConfig, baudRate); //配置波特率
#endif

	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
	MAP_UART_enableModule(EUSCI_A0_BASE);
}

//串口接收缓存区
uint8_t UART2_TX_BUF[UART2_SEND_LEN_MAX]; //发送缓冲,最大UART2_SEND_LEN_MAX字节

//初始化IO 串口2
// bound:波特率
void uart2_init(uint32_t baudRate)
{
#ifdef EUSCI_A_UART_7_BIT_LEN
	//固件库v3_40_01_02
	//默认SMCLK 48MHz 比特率
	const eUSCI_UART_ConfigV1 uartConfig =
		{
			EUSCI_A_UART_CLOCKSOURCE_SMCLK,				   // SMCLK Clock Source
			312,										   // BRDIV = 312
			8,											   // UCxBRF = 8
			1,											   // UCxBRS = 1
			EUSCI_A_UART_NO_PARITY,						   // No Parity
			EUSCI_A_UART_LSB_FIRST,						   // MSB First
			EUSCI_A_UART_ONE_STOP_BIT,					   // One stop bit
			EUSCI_A_UART_MODE,							   // UART mode
			EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
			EUSCI_A_UART_8_BIT_LEN						   // 8 bit data length
		};
	eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //配置波特率
#else
	//固件库v3_21_00_05
	//默认SMCLK 48MHz 比特率
	const eUSCI_UART_Config uartConfig =
		{
			EUSCI_A_UART_CLOCKSOURCE_SMCLK,				   // SMCLK Clock Source
			312,										   // BRDIV = 312
			8,											   // UCxBRF = 8
			1,											   // UCxBRS = 1
			EUSCI_A_UART_NO_PARITY,						   // No Parity
			EUSCI_A_UART_LSB_FIRST,						   // MSB First
			EUSCI_A_UART_ONE_STOP_BIT,					   // One stop bit
			EUSCI_A_UART_MODE,							   // UART mode
			EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
		};
	eusci_calcBaudDividers((eUSCI_UART_Config *)&uartConfig, baudRate); //配置波特率
#endif
	// 1.配置GPIO复用
	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P3, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
	MAP_UART_initModule(EUSCI_A2_BASE, &uartConfig); // 3.初始化串口
	MAP_UART_enableModule(EUSCI_A2_BASE);			 // 4.开启串口模块

	MAP_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT); // 5.开启串口相关中断
	MAP_Interrupt_enableInterrupt(INT_EUSCIA2);								 // 6.开启串口端口中断
}

//串口2,printf 函数
//确保一次发送数据不超过USART3_MAX_SEND_LEN字节
void printf_uart2(char *fmt, ...)
{
	uint16_t i, j;
	va_list ap;
	va_start(ap, fmt);
	vsprintf((char *)UART2_TX_BUF, fmt, ap);
	va_end(ap);
	i = strlen((const char *)UART2_TX_BUF); //此次发送数据的长度
	for (j = 0; j < i; j++)					//循环发送数据
	{
		MAP_UART_transmitData(EUSCI_A2_BASE, UART2_TX_BUF[j]);
	}
}
