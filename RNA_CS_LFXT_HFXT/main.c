#include "sysinit.h"
#include "uartinit.h"

/******************************************************************************
//MSP432P401R
//3-2 配置外部晶振
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/15
*******************************************************************************/

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD; // 停用看门狗

	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN0, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
	GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

	CS_setExternalClockSourceFrequency(32768, 48000000);

	PCM_setCoreVoltageLevel(PCM_VCORE1);

	FlashCtl_setWaitState(FLASH_BANK0, 1);
	FlashCtl_setWaitState(FLASH_BANK1, 1);

	CS_startHFXT(false);
	CS_startLFXT(CS_LFXT_DRIVE3);

	CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);
	CS_initClockSignal(CS_HSMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

	uart_init(115200); //第7讲 串口实验

	printf("MSP432\r\n");
	printf("2021/8/15\r\n");

	printf("ACLK:%d\r\n", CS_getACLK());
	printf("MCLK:%d\r\n", CS_getMCLK());
	printf("SCLK:%d\r\n", CS_getSMCLK());
	printf("HMSCLK:%d\r\n", CS_getHSMCLK());

	while (1)
	{
	}
}
