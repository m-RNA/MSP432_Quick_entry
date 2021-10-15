/****************************************************/
// MSP432P401R
// ��������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************   �汾����˵��   *****************
 * 
 * �˰汾֧�� ��׼C��
 * ���ڿ��Բ�ʹ��΢����
 * 
 * ? ��Ҫע�⣺
 * ��ʹ�ñ�׼C��ʱ�����޷�ʹ��scanf��
 * �����Ҫʹ��scanfʱ����ʹ��΢�� MicroLIB
 * �ٵ�Ƶʱ��Ƶ���£��߲�����ʹ�ô���ʱ������,
 * ����35768Hz��19200������,
 * ��ʹ�ô��������ʱ���Գ��Խ��Ͳ����ʡ�
 * ��baudrate_calculate��������ȥ�ļ��ڲ鿴��
 * 
 * **************************************************
 * 
 * ? v3.0  2021/10/15
 * �˰汾֧��ʹ�� ��׼C��
 * �ļ���ʽ����Ϊ������ԭ��ͬ����
 * usart.c �� usart.h
 * ������ֲ
 *  
 * ? v2.1  2021/8/27
 * ���֧�ֹ̼���v3_21_00_05
 * ��֧�� MicroLIB ΢��
 * 
 * ? v2.0  2021/8/25
 * uart_init�����˲����ʴ����������ֱ�����ò����ʡ�
 * ����UART�Ĵ��뵥�����Ϊ��Ϊ
 * baudrate_calculate��c�ļ���h�ļ�
 * ��֧�� MicroLIB ΢��
 * 
 * ? v1.0 2021/7/17
 * ��֧�ֹ̼���v3_40_01_02
 * ������SMCLK 48MHz ������ 115200�ĳ�ʼ�����룬
 * �Խӱ�׼��������⣬ʹ����ʹ��printf��scanf����
 * ��֧�� MicroLIB ΢��
 * 
 ****************************************************/
 
#include "usart.h"
#include "baudrate_calculate.h"

//Ԥ����
//if 1 ʹ�ñ�׼C�� ��������ʹ��΢��
//if 0 ʹ��΢�� ��ȥ��ѡħ������� Use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
  int handle;
};
FILE __stdout;
//����_sys_exit()�Ա���ʹ�ð�����ģʽ
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

/*****************   ����˵��   *****************
 *
 * ���������Խӱ�׼���������ĺ���:
 * int fputc(int ch, FILE *f);
 * int fgetc(FILE *f);
 * Դ��ΪBiliBiliƽ̨UP�� ��CloudBoyStudio�� ��д
 * ����RNA����������
 * �ڴ�Ҳ���л
 *
 *****************   ˵������   *****************/

void uart_init(uint32_t baudRate)
{
#ifdef EUSCI_A_UART_7_BIT_LEN
  //�̼���v3_40_01_02
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
#else
  //�̼���v3_21_00_05
  //Ĭ��SMCLK 48MHz ������ 115200
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
  eusci_calcBaudDividers((eUSCI_UART_Config *)&uartConfig, baudRate); //���ò�����
#endif

  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
  MAP_UART_enableModule(EUSCI_A0_BASE);
}
