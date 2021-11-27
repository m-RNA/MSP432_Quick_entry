/****************************************************/
// MSP432P401R
// 配置Keil独立空工程 (已将ti固件库打包至工程)
// Keil工程已配置开启FPU
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/28
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "tim32.h"

int main(void)
{
    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    delay_init();      // 第4讲 滴答延时

    /*开始填充初始化代码*/

    /*停止填充初始化代码*/

    printf("Hello,MSP432!\r\n");
    MAP_Interrupt_enableMaster(); // 开启总中断
    while (1)
    {
        /*开始填充用户代码*/

        /*停止填充用户代码*/
    }
}
