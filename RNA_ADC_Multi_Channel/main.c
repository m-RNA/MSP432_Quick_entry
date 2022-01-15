/******************************************************************************
//MSP432P401R
//11 ADC 多通道采集
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/13
*******************************************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "adc.h"

/************************
 *
 * 最大采集电压 3.3V
 * 
 * 实验现象：
 * 串口打印ADC采集电压
 *
 * ADC采集引脚：
 * 单路 为 P5.5
 * 双路 为 P5.5 P5.4
 * 三路 为 P5.5 P5.4 P5.3
 * 可在adc.c中配置采集次数与采集路数
 *
 ************************/

int main(void)
{
    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    delay_init();      // 第4讲 滴答延时
    ADC_Config();      // 第11讲 ADC采集

    printf("Hello MSP432!\r\n");
    while (1)
    {
    }
}
