#include "sysinit.h"
#include "uartinit.h"
#include "led.h"

/******************************************************************************
// MSP432P401R
// 7 串口配置
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/25
*******************************************************************************/

int main(void)
{
    char a[100];
    SysInit();         //第3讲 时钟配置
    uart_init(115200); //第7讲 串口实验
    LED_Init();        //第2讲 GPIO输出

    printf("MSP432\r\n");
    printf("2021/8/24\r\n");

    LED_RED_On();
    while (1)
    {
        scanf("%s", a);
        printf("%s\r\n", a);
    }
}
