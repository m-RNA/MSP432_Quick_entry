/****************************************************/
// MSP432P401R
// 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
/****************************************************/

/******************   版本更新说明   *****************
 * 
 * 此版本CCS支持printf，Keil支持标准C库跟微库
 * 用Keil开发终于可以不开微库啦
 * 
 * ? 需要注意：
 * ①使用标准C库时，将无法使用scanf。
 * 如果需要使用scanf时，请使用微库 MicroLIB
 * ①低频时钟频率下，高波特率使得传输时误差过大,
 * 比如35768Hz下19200波特率,
 * 会使得传输出错，这时可以尝试降低波特率。
 * ②baudrate_calculate的问题请去文件内查看。
 * 
 * **************************************************
 * 
 * ? v3.1  2021/10/18
 * 添加对CCS的支持
 *
 * ? v3.0  2021/10/15
 * 此版本支持使用 标准C库
 * 文件正式改名为与正点原子同名的
 * usart.c 和 usart.h，方便移植
 * 仅支持Keil平台开发
 *  
 * ? v2.1  2021/8/27
 * 添加支持固件库v3_21_00_05
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 * ? v2.0  2021/8/25
 * uart_init增添了波特率传入参数，可直接配置波特率。
 * 计算UART的代码单独打包为名为
 * baudrate_calculate的c文件和h文件
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 * ? v1.0 2021/7/17
 * 仅支持固件库v3_40_01_02
 * 配置了SMCLK 48MHz 波特率 115200的初始化代码，
 * 对接标准输入输出库，使其能使用printf、scanf函数
 * 仅支持 MicroLIB 微库、Keil平台开发
 * 
 ****************************************************/

#include "usart.h"
#include "baudrate_calculate.h"

#ifdef __TI_COMPILER_VERSION__
//CCS平台
//打印字符
void putc(char ch)
{
  UART_transmitData(EUSCI_A0_BASE, ch);
}
//打印字符串
void puts(char *s)
{
  while (*s)
    putc(*s++);
}
//快速幂 + 迭代
//时间复杂度：O(logn)，即为对 nn 进行二进制拆分的时间复杂度
//空间复杂度：O(1)
unsigned long quickMul(unsigned long x, unsigned long N)
{
  unsigned long ans = 1;
  // 贡献的初始值为 x
  unsigned long x_contribute = x;
  // 在对 N 进行二进制拆分的同时计算答案
  while (N > 0)
  {
    if (N % 2 == 1)
    {
      // 如果 N 二进制表示的最低位为 1，那么需要计入贡献
      ans *= x_contribute;
    }
    // 将贡献不断地平方
    x_contribute *= x_contribute;
    // 舍弃 N 二进制表示的最低位，这样我们每次只要判断最低位即可
    N /= 2;
  }
  return ans;
}
static unsigned long m_pow(int x, int n)
{
  unsigned long N = n;
  return quickMul(x, N);
}
// 作者：LeetCode-Solution
// 链接：https://leetcode-cn.com/problems/powx-n/solution/powx-n-by-leetcode-solution/
// 来源：力扣（LeetCode）
// 著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

void printf(char *str, ...)
{
  va_list ap; //定义一个可变 参数的（字符指针）
  int val, r_val;
  char count, ch;
  char *s = NULL;
  int res = 0; //返回值

  va_start(ap, str);   //初始化ap
  while ('\0' != *str) //str为字符串,它的最后一个字符肯定是'\0'（字符串的结束符）
  {
    switch (*str)
    {
    case '%': //发送参数
      str++;
      switch (*str)
      {
      case 'u':
      case 'd': //10进制输出
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
          count++; //整数的长度
          r_val /= 10;
        }
        res += count; //返回值长度增加?
        r_val = val;
        while (count)
        {
          ch = r_val / m_pow(10, count - 1);
          r_val %= m_pow(10, count - 1);
          putc(ch + '0'); //数字到字符的转换
          count--;
        }
        break;
      case 'X': //16进制输出
      case 'x':
        val = va_arg(ap, int);
        r_val = val;
        count = 0;
        while (r_val)
        {
          count++; //整数的长度
          r_val /= 16;
        }
        res += count; //返回值长度增加?
        r_val = val;
        while (count)
        {
          ch = r_val / m_pow(16, count - 1);
          r_val %= m_pow(16, count - 1);
          if (ch <= 9)
            putc(ch + '0'); //数字到字符的转换
          else
            putc(ch - 10 + 'A');
          count--;
        }
        break;
      case 's': //发送字符串
        s = va_arg(ap, char *);
        puts(s);          //字符串,返回值为字符指针
        res += strlen(s); //返回值长度增加 ?
        break;
      case 'c':
        putc((char)va_arg(ap, int)); //大家猜为什么不写char，而要写int
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
    default: //显示原来的第一个参数的字符串(不是..里的参数o)
      putc(*str);
      res += 1;
    }
    str++;
  }
}
/*****************   函数说明   *****************
 *
 * 函数：void printf(char *str, ...)
 * 源码来自: https://github.com/hello-myj/stm32_oled
 * 稍作改动适配CCS工程，在此也表感谢原作者。
 *
 *****************   说明结束   *****************/

#else
//Keil支持标准C库跟微库
//预编译
//if 1 使用标准C库 如果报错就使用微库
//if 0 使用微库 得去勾选魔术棒里的 Use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
  int handle;
};
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
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
/*****************   函数说明   *****************
 *
 * 以上两条对接标准输入输出库的函数:
 * int fputc(int ch, FILE *f);
 * int fgetc(FILE *f);
 * 源码为BiliBili平台UP主 “CloudBoyStudio” 编写
 * 本人RNA，不是作者
 * 在此也表感谢
 *
 *****************   说明结束   *****************/
#endif

void uart_init(uint32_t baudRate)
{
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
  eusci_calcBaudDividers((eUSCI_UART_ConfigV1 *)&uartConfig, baudRate); //配置波特率
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
  eusci_calcBaudDividers((eUSCI_UART_Config *)&uartConfig, baudRate); //配置波特率
#endif

  MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
  MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
  MAP_UART_enableModule(EUSCI_A0_BASE);
}
