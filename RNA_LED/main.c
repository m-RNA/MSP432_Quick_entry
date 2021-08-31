/******************************************************************************
//MSP432P401R
//2-1 GPIO输出 跑马灯
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

int main(void)
{
    uint32_t i;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    LED_Init();
    
    while (1)
    {
        LED_RED_On();
        for (i = 0; i < 500000; i++);
        LED_RED_Off();

        LED_R_On();
        for (i = 0; i < 500000; i++);
        LED_R_Off();

        LED_G_On();
        for (i = 0; i < 500000; i++);
        LED_G_Off();

        LED_B_On();
        for (i = 0; i < 500000; i++);
        LED_B_Off();
    }
}
