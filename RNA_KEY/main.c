/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "led.h"
#include "key.h"

/******************************************************************************
//MSP432P401R
//2-2 GPIO输入 板载按键
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/12
*******************************************************************************/

int main(void)
{
    uint8_t key;

    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    LED_Init();
    KEY_Init();

    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY1_PRES)
            LED_RED_On();
        else if (key == KEY2_PRES)
            LED_RED_Off();
    }
}
