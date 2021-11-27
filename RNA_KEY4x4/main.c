/******************************************************************************
//MSP432P401R
//2-2-3 GPIO输入 外接矩阵键盘
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
*******************************************************************************/

#include "sysinit.h"
#include "uartinit.h"
#include "key4x4.h"

int main(void)
{
    uint8_t key_value;

    SysInit();         //第3讲 时钟配置
    uart_init(115200); //第7讲 串口实验
    KEY4x4_Init();     //第2讲 GPIO输入
	
    printf("MSP432\r\n");
    printf("2021/8/11\r\n");
	
    while (1)
    {
        key_value = KEY4x4_Scan(0);
        if (key_value)
            printf("%d", key_value);
    }
}
