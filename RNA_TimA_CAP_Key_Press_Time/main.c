#include "sysinit.h"
#include "uartinit.h"
#include "timA.h"
#include "led.h"
#include "key.h"

/****************************************************/
// MSP432P401R
// 8-3 定时器捕获
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/9/6
/****************************************************/

/*****************   实验说明   *****************
 *
 * 基于 正点原子@ALIENTEK 的 实验10 输入捕获实验
 * 进行移植，非常感谢。
 * 
 * 需要注意：
 * 变量为16位时，采集高电平的最长时长为4.19s，
 * 超出时会强制标记成功捕获完高电平。
 *
 *****************   说明结束   *****************/

int main(void)
{
    uint8_t key;
    uint32_t temp;

    SysInit();         //第3讲 时钟配置
    uart_init(115200); //第7讲 串口配置
    LED_Init();        //第2讲 GPIO输出
    KEY_Init();        //第2讲 GPIO输入

    TimA2_Cap_Init(); //第8讲 定时器A 捕获

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
            temp *= 65536;                              //溢出时间总和
            temp += TIMA2_CAP_VAL;                      //得到总的高电平时间
            printf("HIGH:%f s\r\n", temp / 1000000.0f); //打印总的高点平时间
            TIMA2_CAP_STA = 0;                          //开启下一次捕获
        }
    }
}
