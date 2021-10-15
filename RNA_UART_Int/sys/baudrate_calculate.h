/****************************************************/
// MSP432P401R
// 串口波特率计算
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************************    说明    ******************************
 *
 * 源码为TI官方编写,本人只是将JS程序移植到了C语言平台,仅作为学习使用。源码出处为：
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * 
 * ? 已知问题：
 * 调试时发现某些情况下，C语言的小数的大小与JS的相差较大，
 * 导致了算出的UCSx(即secondModReg)不一样，
 * 这时如果出现不能准确传输时，请换一个波特率。
 *
 * ? 需要注意：
 * 波特率不能大于时钟频率，否则会退出函数
 * 
 * *****************************   版本说明   ******************************
 * 
 * ? v1.2 2021/8/29
 * 注释掉了闪烁灯的代码
 * 
 * ? v1.1  2021/8/27
 * 添加支持固件库v3_21_00_05 
 * 
 * ? v1.0  2021/8/25
 * 仅支持固件库v3_40_01_02
 * 
 * *******************************   结束    *******************************/
 
 #ifndef __RNA_BAUDRATE_CALCULATE_H
#define __RNA_BAUDRATE_CALCULATE_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

//错误指示灯宏定义 方便移植使用
//MSP432P401R 有两个红灯P1.0 P2.0
//#define WARN_LED_1_PORT GPIO_PORT_P1
//#define WARN_LED_2_PORT GPIO_PORT_P2
//#define WARN_LED_1_PIN GPIO_PIN0
//#define WARN_LED_2_PIN GPIO_PIN0
//#define WARN_LED_INIT MAP_GPIO_setAsOutputPin
//#define WARN_LED_ON MAP_GPIO_setOutputHighOnPin
//#define WARN_LED_OFF MAP_GPIO_setOutputLowOnPin

#ifdef EUSCI_A_UART_7_BIT_LEN
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uart_config, uint32_t baudRate); //固件库v3_40_01_02
#else
void eusci_calcBaudDividers(eUSCI_UART_Config *uart_config, uint32_t baudRate); //固件库v3_21_00_05
#endif

#endif
