/******************************************************************************
//MSP432P401R
//6 �ж����ȼ�����
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/22
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

	//1.����GPIO����
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); //P1.1
	GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P2, GPIO_PIN4); //P2.4

	//2.����жϱ�־λ
	GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_clearInterruptFlag(GPIO_PORT_P2, GPIO_PIN4);

	//3.���ô�����ʽ
	GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);
	GPIO_interruptEdgeSelect(GPIO_PORT_P2, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION);

	//4.5 ���������ȼ�
	Interrupt_setPriority(INT_PORT1, 1 << 5);
	Interrupt_setPriority(INT_PORT2, 2 << 5);

	//4.�����ⲿ�ж�
	GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
	GPIO_enableInterrupt(GPIO_PORT_P2, GPIO_PIN4);

	//5.�����˿��ж�
	Interrupt_enableInterrupt(INT_PORT1);
	Interrupt_enableInterrupt(INT_PORT2);

	//6.�������ж�
	Interrupt_enableMaster();

	while (1)
	{
	}
}

//7.��дGPIO ISR
void PORT1_IRQHandler(void)
{
	uint16_t status;
	uint16_t i;
	status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
	GPIO_clearInterruptFlag(GPIO_PORT_P1, status);
	for (i = 0; i < 5000; i++);
	
	if (status & GPIO_PIN1) //��ӦP1.1
	{
		if (KEY1 == 0)
		{
			LED_RED_On(); //�������
			while (1);
		}
	}
}

void PORT2_IRQHandler(void)
{
	uint16_t status;
	uint16_t i;
	status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P2);
	GPIO_clearInterruptFlag(GPIO_PORT_P2, status);
	for (i = 0; i < 5000; i++);
	
	if (status & GPIO_PIN4) //��ӦP2.4
	{
		if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN4) == 0)
		{
			LED_B_On(); //��������
			while (1);
		}
	}
}
