#include "sysinit.h"
#include "uartinit.h"
#include "delay.h"
#include "adc.h"

/******************************************************************************
//MSP432P401R
//11 ADC 多通道采集
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/13
*******************************************************************************/

int main(void)
{
    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    delay_init();      // 第4讲 滴答延时
	
	ADC_Config(); // 第11讲 ADC采集 
	
    printf("Hello MSP432!\r\n");
	
    while (1)
    {
    }
}

