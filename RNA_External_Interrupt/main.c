/******************************************************************************
//MSP432P401R
//5 外部中断 课件使用
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/21
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

    LED_Init();
	
	//1.配置GPIO输入
	KEY_Init();

	//2.清除中断标志位
	GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN1);
	GPIO_clearInterruptFlag(GPIO_PORT_P1,GPIO_PIN4);
	
	//3.配置触发方式
	GPIO_interruptEdgeSelect(GPIO_PORT_P1,GPIO_PIN1,GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_interruptEdgeSelect(GPIO_PORT_P1,GPIO_PIN4,GPIO_HIGH_TO_LOW_TRANSITION);
	
	//4.开启外部中断
	GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P1,GPIO_PIN4);
	
	//5.开启端口中断
	Interrupt_enableInterrupt(INT_PORT1);
	
	//6.开启总中断
	Interrupt_enableMaster();
	
    while (1)
    {
    }
}

//7.编写GPIO ISR
void PORT1_IRQHandler(void)
{
	uint16_t status;
	uint16_t i;
	status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
	GPIO_clearInterruptFlag(GPIO_PORT_P1,status);
	for(i = 0; i<5000;i++);
	
	if(status & GPIO_PIN1)
	{		
		if(KEY1 == 0)
		{
			LED_RED_On();
		}
	}

	if(status & GPIO_PIN4)
	{
		if(KEY2 == 0)
		{
			LED_RED_Off();
		}
	}
}



