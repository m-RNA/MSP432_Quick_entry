/******************************************************************************
//MSP432P401R
//5 外部中断
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
*******************************************************************************/

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "led.h"
#include "key.h"

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    LED_Init(); //第2讲 GPIO输出
    KEY_Init(1);//第5讲 外部中断

    MAP_Interrupt_enableMaster(); //开启总中断

    while (1)
    {
    }
}
