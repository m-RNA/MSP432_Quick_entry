#ifndef __UARTINIT_H
#define __UARTINIT_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h> //1.61328125kb

/****************************************************/
// MSP432P401R
// 串口配置
// 版本 v2.0
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/25
/****************************************************/

/******************   版本更新说明   *****************
 * 
 * ? v2.0  2021/8/25
 * uart_init增添了波特率传入参数，可直接配置波特率。
 * 计算UART的代码单独打包为名为
 * baudrate_calculate的c文件和h文件,
 * ? 需要注意：
 * ①低频时钟频率下，高波特率使得传输时误差过大,
 * 比如35768Hz下19200波特率,
 * 会使得传输出错，这时可以尝试降低波特率。
 * ②baudrate_calculate的问题请去文件内查看。
 * 
 * ? v1.0 2021/8/11
 * 配置了SMCLK 48MHz 波特率 115200的初始化代码，
 * 对接标准输入输出库，使其能使用printf、scanf函数
 * 
 ****************************************************/
void uart_init(uint32_t baudRate);

#endif
