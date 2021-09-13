#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//�����Ҫʹ��OS,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos ʹ��
#endif
/**************************************************************************************/
/*************                   ����ԭ��@ALIENTEK                **********************/
//������̳:www.openedv.com
//��������:2010/1/1
//�汾��V1.8
/* ˵�� ��
 * Դ��Ϊ����ԭ��@ALIENTEK ��д
 * ����RNA��ֻ����ֲ����MSP432ƽ̨
 * ����Ϊѧϰʹ��
 * �ڴ�Ҳ��л����ԭ��
 */
/**************************************************************************************/

/******************************************************************************
//MSP432P401R
//4 ��ֲ�δ���ʱ
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/7/22
*******************************************************************************/

static uint8_t fac_us = 0;	//us��ʱ������
static uint16_t fac_ms = 0; //ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��

#if SYSTEM_SUPPORT_OS //���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
//��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
//������3���궨��:
//    delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
// delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
//Ȼ����3������:
//  delay_osschedlock:��������OS�������,��ֹ����
//delay_osschedunlock:���ڽ���OS�������,���¿�������
//    delay_ostimedly:����OS��ʱ,���������������.

//�����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef OS_CRITICAL_METHOD					 //OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII
#define delay_osrunning OSRunning			 //OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec OS_TICKS_PER_SEC //OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting OSIntNesting		 //�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef CPU_CFG_CRITICAL_METHOD				  //CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII
#define delay_osrunning OSRunning			  //OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec OSCfg_TickRate_Hz //OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting OSIntNestingCtr	  //�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD //ʹ��UCOSIII
	OS_ERR err;
	OSSchedLock(&err); //UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else				   //����UCOSII
	OSSchedLock();	  //UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD //ʹ��UCOSIII
	OS_ERR err;
	OSSchedUnlock(&err); //UCOSIII�ķ�ʽ,�ָ�����
#else					 //����UCOSII
	OSSchedUnlock();  //UCOSII�ķ�ʽ,�ָ�����
#endif
}

//����OS�Դ�����ʱ������ʱ
//ticks:��ʱ�Ľ�����
void delay_ostimedly(uint32_t ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err;
	OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err); //UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(ticks); //UCOSII��ʱ
#endif
}

//systick�жϷ�����,ʹ��ucosʱ�õ�
void SysTick_Handler(void)
{
	if (delay_osrunning == 1) //OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter(); //�����ж�
		OSTimeTick(); //����ucos��ʱ�ӷ������
		OSIntExit();  //���������л����ж�
	}
}
#endif

//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init()
{
#if SYSTEM_SUPPORT_OS //�����Ҫ֧��OS.
	uint32_t reload;
#endif
	fac_us = CS_getMCLK() / 1000000;		 //ϵͳʱ��
#if SYSTEM_SUPPORT_OS						 //�����Ҫ֧��OS.
	reload = SystemCoreClock / 8000000;		 //ÿ���ӵļ������� ��λΪK
	reload *= 1000000 / delay_ostickspersec; //����delay_ostickspersec�趨���ʱ��
											 //reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����
	fac_ms = 1000 / delay_ostickspersec;	 //����OS������ʱ�����ٵ�λ

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //����SYSTICK�ж�
	SysTick->LOAD = reload;					   //ÿ1/delay_ostickspersec���ж�һ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //����SYSTICK

#else
	fac_ms = (uint16_t)fac_us * 1000; //��OS��,����ÿ��ms��Ҫ��systickʱ����
#endif
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //�ڲ�ʱ��Դ
}

#if SYSTEM_SUPPORT_OS //�����Ҫ֧��OS.
//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD��ֵ
	ticks = nus * fac_us;			 //��Ҫ�Ľ�����
	tcnt = 0;
	delay_osschedlock(); //��ֹOS���ȣ���ֹ���us��ʱ
	told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
	delay_osschedunlock(); //�ָ�OS����
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(uint16_t nms)
{
	if (delay_osrunning && delay_osintnesting == 0) //���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)
	{
		if (nms >= fac_ms) //��ʱ��ʱ�����OS������ʱ������
		{
			delay_ostimedly(nms / fac_ms); //OS��ʱ
		}
		nms %= fac_ms; //OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ
	}
	delay_us((uint32_t)(nms * 1000)); //��ͨ��ʽ��ʱ
}
#else //����OSʱ
//��ʱnus
//nusΪҪ��ʱ��us��.
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = nus * fac_us;			  //ʱ�����
	SysTick->VAL = 0x00;					  //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//�رռ�����
	SysTick->VAL = 0X00;							//��ռ�����
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864
void delay_ms(uint16_t nms)
{
	uint32_t temp;
	SysTick->LOAD = (uint32_t)nms * fac_ms;	  //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;					  //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//�رռ�����
	SysTick->VAL = 0X00;							//��ռ�����
}
#endif
