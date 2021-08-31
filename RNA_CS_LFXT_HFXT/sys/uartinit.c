#include "uartinit.h"
/****************************************************/
//MSP432P401R
//��������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
/****************************************************/

/**************************************************************************************/
/*************                    CloudBoyStudio                 **********************/
int fputc(int ch, FILE *f)
{
    UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
    return ch;
}

int fgetc(FILE *f)
{
    while (EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG !=
           UART_getInterruptStatus(
               EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        ;
    return UART_receiveData(EUSCI_A0_BASE);
}
/* ˵�� ��
 * ������������:
 * int fputc(int ch, FILE *f); 
 * int fgetc(FILE *f);
 * Դ��ΪBiliBiliƽ̨UP�� CloudBoyStudio ��д
 * ����RNA����������
 * �ڴ�Ҳ���л
 * ʹ��ʱ�ǵù�ѡħ�������Use MicroLIB
 */
/**************************************************************************************/

//SMCLK 48MHz   ������ 115200
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

//SMCLK 48MHz   ������ 115200
void UART_Init(void)
{
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);
}
