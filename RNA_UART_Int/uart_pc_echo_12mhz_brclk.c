/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "sysinit.h"
#include "baudrate_calculate.h"

int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    //0.配置时钟
    SysInit();

    //1.配置GPIO复用
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    //2.配置UART结构体
#ifdef EUSCI_A_UART_7_BIT_LEN
    //固件库v3_40_01_02
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
    eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, 115200); //配置波特率
#else
    //固件库v3_21_00_05
    //默认SMCLK 48MHz 比特率 115200
    const eUSCI_UART_Config uartConfig =
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
        };
    eusci_calcBaudDividers((eUSCI_UART_Config *)&uartConfig, 115200); //配置波特率
#endif
    //3.初始化串口
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    //4.开启串口模块
    UART_enableModule(EUSCI_A0_BASE);
	
    //5.开启串口相关中断
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);

    //6.开启串口端口中断
    Interrupt_enableInterrupt(INT_EUSCIA0);

    //7.开启总中断
    Interrupt_enableMaster();

    while(1)
    {
    }
}

//8.编写UART ISR
void EUSCIA0_IRQHandler(void)
{
	
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) //接收中断
    {
        UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE)); //发送数据
    }

}

