/*
 * delay.c
 *
 *  Created on: 2016��1��29��
 *      Author: Mike
 */

#include "delay.h"


static uint8_t  fac_us=0;  //us��ʱ������
static uint16_t fac_ms=0;  //ms��ʱ������

//void delay_init(uint32_t SystemCoreClock)
//{
//	fac_us = SystemCoreClock / 1000000;    //�����us��ϵͳƵ���й�
//	//fac_us = 48;
//	fac_ms = (uint16_t)fac_us*1000;      //ms��Ϊus��1000��
//}
void delay_init(void)
{
	fac_us = CS_getMCLK() / 1000000;    //�����us��ϵͳƵ���й�
	//fac_us = 48;
	fac_ms = (uint16_t)fac_us*1000;      //ms��Ϊus��1000��
}

void delay_us(uint32_t nus)
{
	uint32_t temp;

	SysTick->LOAD=(uint32_t)(nus*fac_us);  //STRVR ��װ�ؼĴ�����������
	SysTick->VAL=0x00;         //STCVR ��λ��ǰ�Ĵ�������
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;  //STCSR ״̬�Ĵ�������
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD=(uint32_t)(nms*fac_ms);
	SysTick->VAL =0x00;
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL =0X00;
}

