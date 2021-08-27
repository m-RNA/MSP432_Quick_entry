#include "uartinit.h"
#include "baudrate_calculate.h"

/****************************************************/
// MSP432P401R
// ��������
// �汾 v2.0
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/25
/****************************************************/

/******************   �汾����˵��   *****************
 * 
 * ? v2.0  2021/8/25
 * ����������Ϊ��uart_init���������˲����ʴ��������
 * ��ֱ�����ò����ʣ�������ԭ�ӷ�װ������ͳһ��
 * ����UART�Ĵ��뵥�����Ϊ��Ϊ
 * baudrate_calculate��c�ļ���h�ļ���
 * ? ��Ҫע�⣺
 * �ٵ�Ƶʱ��Ƶ���£��߲�����ʹ�ô���ʱ������
 * ����35768Hz��19200�����ʣ�
 * ��ʹ�ô��������ʱ���Գ��Խ��Ͳ����ʡ�
 * ��baudrate_calculate��������ȥ�ļ��ڲ鿴��
 * 
 * ? v1.0 2021/8/11
 * ������SMCLK 48MHz ������ 115200�ĳ�ʼ�����룬
 * �Խӱ�׼��������⣬ʹ����ʹ��printf��scanf����
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
/*****************   ����˵��   *****************
 * 
 * ���������Խӱ�׼���������ĺ���:
 * int fputc(int ch, FILE *f);
 * int fgetc(FILE *f);
 * Դ��ΪBiliBiliƽ̨UP�� ��CloudBoyStudio�� ��д
 * ����RNA����������
 * �ڴ�Ҳ���л
 * ʹ��ʱ�ǵù�ѡħ�������Use MicroLIB
 * 
 *****************   ˵������   *****************/

/**************************************************************************************/

void uart_init(uint32_t baudRate)
{
    //Ĭ��SMCLK 48MHz ������ 115200
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
    eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //���ò�����

    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);UART_enableModule(EUSCI_A0_BASE);
}
