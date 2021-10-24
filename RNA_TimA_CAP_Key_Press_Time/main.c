/****************************************************/
// MSP432P401R
// 8-3 定时器捕获
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/6
/****************************************************/

/**************   实验说明   **************
 * 
 * 声明：
 * 基于正点原子@ALIENTEK STM32
 * 实验10 输入捕获实验 进行移植
 * 在此表示感谢 代码仅供学习使用
 * 
 * 操作：
 * ★ 用杜邦线将 P5.6 和 P1.0 连接起来。
 * ★ 用两个按键来控制高电平持续时间
 * 
 * 需要注意：
 * 这里采集高电平的最长时长为4.19s
 * 超出时会强制标记成功捕获完高电平。
 *
 **************   说明结束   **************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "led.h"
#include "key.h"
#include "delay.h"

int main(void)
{
    uint8_t key;
    uint32_t temp;

    SysInit();         //第3讲 时钟配置
    uart_init(115200); //第7讲 串口配置
    LED_Init();        //第2讲 GPIO输出
    KEY_Init(0);       //第2讲 GPIO输入
    delay_init();      //第4讲 滴答延时
    TimA2_Cap_Init();  //第8讲 定时器A 捕获

    printf("\r\n\r\nHello,MSP432!\r\n");
    MAP_Interrupt_enableMaster();
    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY1_PRES)
            LED_RED_On();
        else if (key == KEY2_PRES)
            LED_RED_Off();

        if (TIMA2_CAP_STA & 0X80) //成功捕获到了一次上升沿
        {
            temp = TIMA2_CAP_STA & 0X3F;
            temp *= 65536;                 //溢出时间总和
            temp += TIMA2_CAP_VAL;         //得到总的高电平时间
            printf("HIGH:%dus\r\n", temp); //打印总的高点平时间
            TIMA2_CAP_STA = 0;             //开启下一次捕获
        }
    }
}
