/******************************************************************************
//MSP432P401R
//10 OLED图形库（硬件IIC/模拟IIC）
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/17
*******************************************************************************/

/*********  声明  **********
 *
 * 0.96寸OLED
 * UP将OLED图形库驱动
 * 移植到了MSP432上来了
 * 项目地址：https://github.com/hello-myj/stm32_oled
 
 * 支持模拟IIC、硬件IIC
 * 引脚接法:
 * P1.7 SCL
 * P1.6 SDA
 * 可以去头文件更改引脚
 *
 ****************************/

#include "sysinit.h"
#include "delay.h"
#include "test.h"
#include "draw_api.h"

int main(void)
{
    SysInit();
    delay_init();
    InitGraph();
    delay_ms(200);
    ClearScreen(); //清屏
    while (1)
    {
        demo();
        ZJY_Demo(); //OLED_TEST里的例子
    }
}
