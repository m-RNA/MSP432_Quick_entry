/****************************************************/
// MSP432P401R
// 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************   版本更新说明   *****************
 * 
 * CCS支持printf
 * Keil支持标准C库跟微库
 * 用Keil开发终于可以不开微库啦
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
 * ? v3.1  2021/10/18
 * 添加对CCS的支持
 *
 * ? v3.0  2021/10/15
 * 此版本支持使用 标准C库
 * 文件正式改名为与正点原子同名的
 * usart.c 和 usart.h，方便移植
 * 仅支持Keil平台开发
 *  
 * ? v2.1  2021/8/27
 * 添加支持固件库v3_21_00_05
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 * ? v2.0  2021/8/25
 * uart_init增添了波特率传入参数，可直接配置波特率。
 * 计算UART的代码单独打包为名为
 * baudrate_calculate的c文件和h文件
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 * ? v1.0 2021/7/17
 * 仅支持固件库v3_40_01_02
 * 配置了SMCLK 48MHz 波特率 115200的初始化代码，
 * 对接标准输入输出库，使其能使用printf、scanf函数
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 ****************************************************/

#ifndef __USART_H
#define __USART_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#ifdef __TI_COMPILER_VERSION__
//CCS平台
#include "stdarg.h"
#include "string.h"
void printf(char *str, ...);
#else
//Keil平台
#include "stdio.h" //1.61328125kb
#endif

void uart_init(uint32_t baudRate);

#endif
