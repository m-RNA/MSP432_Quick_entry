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
