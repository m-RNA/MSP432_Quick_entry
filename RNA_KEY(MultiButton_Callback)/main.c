/****************************************************/
//MSP432P401R
//2-2* GPIO输入(MultiButton版)
//项目地址 https://github.com/0x1abin/MultiButton
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/10/29
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "MultiTimer.h"
#include "key.h"
#include "led.h"

// 前往key.c注册按键事件
// 将代码烧录到板子后，单击、双击、长按按键
// 观察串口打印回的值

int main(void)
{
    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    MultiTimerInit();  // 第4*讲 软件定时器+滴答延时
    KEY_Init();        // 第2讲 GPIO输入(MultiButton)
    LED_Init();        // 第2讲 GPIO输出

    /*开始填充初始化代码*/

    /*停止填充初始化代码*/

    printf("Hello,MSP432!\r\n");

    MAP_Interrupt_enableMaster(); // 开启总中断
    while (1)
    {
        MultiTimerYield(); //必须在主循环调用定时器后台处理函数

        /*开始填充用户代码*/

        /*停止填充用户代码*/
    }
}
