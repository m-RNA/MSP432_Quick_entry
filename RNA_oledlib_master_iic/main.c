#include "sysinit.h"
#include "uartinit.h"
#include "delay.h"
#include "bmp.h"
#include "test.h"
#include "draw_api.h"

/******************************************************************************
//MSP432P401R
//10 OLED图形库（模拟IIC）
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/17
*******************************************************************************/

/*********  声明  **********
 *
 * 0.96寸OLED
 * UP将OLED图形库驱动
 * 移植到了MSP432上来了
 * 目前是移植了模拟IIC方式（传输比较慢，但是方便改引脚）
 * 后续会添加硬件IIC方式等
 * 
 * 项目地址：https://github.com/hello-myj/stm32_oled
 *
 * 引脚接法:
 * P6.0 SCL
 * P6.1 SDA
 * 可以去头文件更改引脚
****************************/

int main(void)
{
    SysInit();
    UartInit();
    delay_init();
    InitGraph();

    while (1)
    {
        demo();
    }
}
