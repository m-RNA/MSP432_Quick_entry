/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

#include "led.h"
#include "key.h"
/******************************************************************************
//MSP432P401R
//5 �ⲿ�ж�
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
*******************************************************************************/

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    LED_Init(); //��2�� GPIO���
    KEY_Init(1);//��5�� �ⲿ�ж�

    MAP_Interrupt_enableMaster(); //�������ж�

    while (1)
    {
    }
}
