#include "uartinit.h"
#include "baudrate_calculate.h"

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
 * 函数名改名为：uart_init，并增添了波特率传入参数，
 * 可直接配置波特率，与正点原子封装代码风格统一。
 * 计算UART的代码单独打包为名为
 * baudrate_calculate的c文件和h文件，
 * ? 需要注意：
 * ①低频时钟频率下，高波特率使得传输时误差过大，
 * 比如35768Hz下19200波特率，
 * 会使得传输出错，这时可以尝试降低波特率。
 * ②baudrate_calculate的问题请去文件内查看。
 * 
 * ? v1.0 2021/8/11
 * 配置了SMCLK 48MHz 波特率 115200的初始化代码，
 * 对接标准输入输出库，使其能使用printf、scanf函数
 * 
 ****************************************************/

/**************************************************************************************/
/****************************       CloudBoyStudio        *****************************/
int fputc(int ch, FILE *f)
{
    UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
    return ch;
}

int fgetc(FILE *f)
{
    while (EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG !=
           UART_getInterruptStatus(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        ;
    return UART_receiveData(EUSCI_A0_BASE);
}
/*****************   函数说明   *****************
 * 
 * 以上两条对接标准输入输出库的函数:
 * int fputc(int ch, FILE *f);
 * int fgetc(FILE *f);
 * 源码为BiliBili平台UP主 “CloudBoyStudio” 编写
 * 本人RNA，不是作者
 * 在此也表感谢
 * 使用时记得勾选魔术棒里的Use MicroLIB
 * 
 *****************   说明结束   *****************/

/**************************************************************************************/

void uart_init(uint32_t baudRate)
{
    //默认SMCLK 48MHz 比特率 115200
    const eUSCI_UART_ConfigV1 uartConfig =
        {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,                // SMCLK Clock Source
            26,                                            // BRDIV = 26
            0,                                             // UCxBRF = 0
            111,                                           // UCxBRS = 111
            EUSCI_A_UART_NO_PARITY,                        // No Parity
            EUSCI_A_UART_LSB_FIRST,                        // MSB First
            EUSCI_A_UART_ONE_STOP_BIT,                     // One stop bit
            EUSCI_A_UART_MODE,                             // UART mode
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
            EUSCI_A_UART_8_BIT_LEN                         // 8 bit data length
        };
    eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //配置波特率

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);UART_enableModule(EUSCI_A0_BASE);
}
