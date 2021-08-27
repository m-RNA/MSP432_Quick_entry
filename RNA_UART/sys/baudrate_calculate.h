#ifndef __RNA_BAUDRATE_CALCULATE_H
#define __RNA_BAUDRATE_CALCULATE_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
/****************************************************/
// MSP432P401R
// 串口波特率计算
// 版本 v1.0
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/25
/****************************************************/

/* ************   说明   ************
 *
 * 源码为TI官方编写,本人只是将JS程序
 * 移植到了C语言平台,仅作为学习使用。
 * 源码出处为：http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * 
 * v1.0 已知问题：
 * 调试时发现C语言的小数的大小与官方JS的不一样，
 * 导致了算出的UCSx(即secondModReg)不一样，
 * 这时如果出现不能准确传输时，请换一个波特率。
 *
 * ************   结束    ************/

//错误指示灯宏定义 方便移植使用
//MSP432P401R 有两个红灯P1.0 P2.0
#define WARN_LED_1_PORT GPIO_PORT_P1
#define WARN_LED_1_PIN GPIO_PIN0
#define WARN_LED_2_PORT GPIO_PORT_P2
#define WARN_LED_2_PIN GPIO_PIN0
#define WARN_LED_1_INIT() MAP_GPIO_setAsOutputPin(WARN_LED_1_PORT, WARN_LED_1_PIN)
#define WARN_LED_2_INIT() MAP_GPIO_setAsOutputPin(WARN_LED_2_PORT, WARN_LED_2_PIN)
#define WARN_LED_1_ON() MAP_GPIO_setOutputHighOnPin(WARN_LED_1_PORT, WARN_LED_1_PIN);
#define WARN_LED_1_OFF() MAP_GPIO_setOutputLowOnPin(WARN_LED_1_PORT, WARN_LED_1_PIN);
#define WARN_LED_2_ON() MAP_GPIO_setOutputHighOnPin(WARN_LED_2_PORT, WARN_LED_2_PIN);
#define WARN_LED_2_OFF() MAP_GPIO_setOutputLowOnPin(WARN_LED_2_PORT, WARN_LED_2_PIN);

/******************   版本说明   *****************
 * 
 * ? v1.0  2021/8/25
 * 
 * ? 需要注意：
 * 波特率不能大于时钟频率，否则程序将会转到闪烁错误指示灯的死循环。
 * 
 * 
 ****************************************************/
void eusci_calcBaudDividers(eUSCI_UART_ConfigV1 *uartConfig, uint32_t baudRate);

#endif
