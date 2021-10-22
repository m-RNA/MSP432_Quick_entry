/****************************************************/
// MSP432P401R
// ��������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************   �汾����˵��   *****************
 * 
 * �˰汾CCS֧��printf��Keil֧�ֱ�׼C���΢��
 * ��Keil�������ڿ��Բ���΢����
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
 * ? v3.1  2021/10/18
 * ��Ӷ�CCS��֧��
 *
 * ? v3.0  2021/10/15
 * �˰汾֧��ʹ�� ��׼C��
 * �ļ���ʽ����Ϊ������ԭ��ͬ����
 * usart.c �� usart.h��������ֲ
 * ��֧��Keilƽ̨����
 *  
 * ? v2.1  2021/8/27
 * ���֧�ֹ̼���v3_21_00_05
 * ��֧�� MicroLIB ΢�⡢Keilƽ̨����
 * 
 * ? v2.0  2021/8/25
 * uart_init�����˲����ʴ����������ֱ�����ò����ʡ�
 * ����UART�Ĵ��뵥�����Ϊ��Ϊ
 * baudrate_calculate��c�ļ���h�ļ�
 * ��֧�� MicroLIB ΢�⡢Keilƽ̨����
 * 
 * ? v1.0 2021/7/17
 * ��֧�ֹ̼���v3_40_01_02
 * ������SMCLK 48MHz ������ 115200�ĳ�ʼ�����룬
 * �Խӱ�׼��������⣬ʹ����ʹ��printf��scanf����
 * ��֧�� MicroLIB ΢�⡢Keilƽ̨����
 * 
 ****************************************************/

#include "usart.h"
#include "baudrate_calculate.h"

#ifdef __TI_COMPILER_VERSION__
//CCSƽ̨
//��ӡ�ַ�
void putc(char ch)
{
  UART_transmitData(EUSCI_A0_BASE, ch);
}
//��ӡ�ַ���
void puts(char *s)
{
  while (*s)
    putc(*s++);
}
//������ + ����
//ʱ�临�Ӷȣ�O(logn)����Ϊ�� nn ���ж����Ʋ�ֵ�ʱ�临�Ӷ�
//�ռ临�Ӷȣ�O(1)
unsigned long quickMul(unsigned long x, unsigned long N)
{
  unsigned long ans = 1;
  // ���׵ĳ�ʼֵΪ x
  unsigned long x_contribute = x;
  // �ڶ� N ���ж����Ʋ�ֵ�ͬʱ�����
  while (N > 0)
  {
    if (N % 2 == 1)
    {
      // ��� N �����Ʊ�ʾ�����λΪ 1����ô��Ҫ���빱��
      ans *= x_contribute;
    }
    // �����ײ��ϵ�ƽ��
    x_contribute *= x_contribute;
    // ���� N �����Ʊ�ʾ�����λ����������ÿ��ֻҪ�ж����λ����
    N /= 2;
  }
  return ans;
}
static unsigned long m_pow(int x, int n)
{
  unsigned long N = n;
  return quickMul(x, N);
}
// ���ߣ�LeetCode-Solution
// ���ӣ�https://leetcode-cn.com/problems/powx-n/solution/powx-n-by-leetcode-solution/
// ��Դ�����ۣ�LeetCode��
// ����Ȩ���������С���ҵת������ϵ���߻����Ȩ������ҵת����ע��������

void printf(char *str, ...)
{
  va_list ap; //����һ���ɱ� �����ģ��ַ�ָ�룩
  int val, r_val;
  char count, ch;
  char *s = NULL;
  int res = 0; //����ֵ

  va_start(ap, str);   //��ʼ��ap
  while ('\0' != *str) //strΪ�ַ���,�������һ���ַ��϶���'\0'���ַ����Ľ�������
  {
    switch (*str)
    {
    case '%': //���Ͳ���
      str++;
      switch (*str)
      {
      case 'u':
      case 'd': //10�������
        val = va_arg(ap, int);
        if (*str == 'd' && val < 0)
        {
          val = -val;
          putc('-');
        }
        r_val = val;
        count = 0;
        while (r_val)
        {
          count++; //�����ĳ���
          r_val /= 10;
        }
        res += count; //����ֵ��������?
        r_val = val;
        while (count)
        {
          ch = r_val / m_pow(10, count - 1);
          r_val %= m_pow(10, count - 1);
          putc(ch + '0'); //���ֵ��ַ���ת��
          count--;
        }
        break;
      case 'X': //16�������
      case 'x':
        val = va_arg(ap, int);
        r_val = val;
        count = 0;
        while (r_val)
        {
          count++; //�����ĳ���
          r_val /= 16;
        }
        res += count; //����ֵ��������?
        r_val = val;
        while (count)
        {
          ch = r_val / m_pow(16, count - 1);
          r_val %= m_pow(16, count - 1);
          if (ch <= 9)
            putc(ch + '0'); //���ֵ��ַ���ת��
          else
            putc(ch - 10 + 'A');
          count--;
        }
        break;
      case 's': //�����ַ���
        s = va_arg(ap, char *);
        puts(s);          //�ַ���,����ֵΪ�ַ�ָ��
        res += strlen(s); //����ֵ�������� ?
        break;
      case 'c':
        putc((char)va_arg(ap, int)); //��Ҳ�Ϊʲô��дchar����Ҫдint
        res += 1;
        break;
      default:;
      }
      break;
    case '\n':
      putc('\n');
      res += 1;
      break;
    case '\r':
      putc('\r');
      res += 1;
      break;
    default: //��ʾԭ���ĵ�һ���������ַ���(����..��Ĳ���o)
      putc(*str);
      res += 1;
    }
    str++;
  }
}
/*****************   ����˵��   *****************
 *
 * ������void printf(char *str, ...)
 * Դ������: https://github.com/hello-myj/stm32_oled
 * �����Ķ�����CCS���̣��ڴ�Ҳ���лԭ���ߡ�
 *
 *****************   ˵������   *****************/

#else
//Keil֧�ֱ�׼C���΢��
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
#endif

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
