/*******************************************
//MSP432P401R
//4 移植滴答延时
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/7/22
*******************************************/

/********      正点原子@ALIENTEK      ********
 * 技术论坛:www.openedv.com
 * 创建日期:2010/1/1
 * 版本：V1.8
 *
 * 说明 ：
 * 源码为正点原子@ALIENTEK 编写
 * 本人RNA，只是移植到了MSP432平台
 * 仅作为学习使用
 * 在此也感谢正点原子
 *********************************************/

#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////
//如果需要使用OS,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h" //ucos 使用
static uint16_t fac_ms = 0; //ms延时倍乘数,在ucos下,代表每个节拍的ms数
#endif
static uint8_t fac_us = 0;	//us延时倍乘数


#if SYSTEM_SUPPORT_OS //如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).
//当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
//首先是3个宏定义:
//    delay_osrunning:用于表示OS当前是否正在运行,以决定是否可以使用相关函数
//delay_ostickspersec:用于表示OS设定的时钟节拍,delay_init将根据这个参数来初始哈systick
// delay_osintnesting:用于表示OS中断嵌套级别,因为中断里面不可以调度,delay_ms使用该参数来决定如何运行
//然后是3个函数:
//  delay_osschedlock:用于锁定OS任务调度,禁止调度
//delay_osschedunlock:用于解锁OS任务调度,重新开启调度
//    delay_ostimedly:用于OS延时,可以引起任务调度.

//本例程仅作UCOSII和UCOSIII的支持,其他OS,请自行参考着移植
//支持UCOSII
#ifdef OS_CRITICAL_METHOD					 //OS_CRITICAL_METHOD定义了,说明要支持UCOSII
#define delay_osrunning OSRunning			 //OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec OS_TICKS_PER_SEC //OS时钟节拍,即每秒调度次数
#define delay_osintnesting OSIntNesting		 //中断嵌套级别,即中断嵌套次数
#endif

//支持UCOSIII
#ifdef CPU_CFG_CRITICAL_METHOD				  //CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII
#define delay_osrunning OSRunning			  //OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec OSCfg_TickRate_Hz //OS时钟节拍,即每秒调度次数
#define delay_osintnesting OSIntNestingCtr	  //中断嵌套级别,即中断嵌套次数
#endif

//us级延时时,关闭任务调度(防止打断us级延迟)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD //使用UCOSIII
	OS_ERR err;
	OSSchedLock(&err); //UCOSIII的方式,禁止调度，防止打断us延时
#else				   //否则UCOSII
	OSSchedLock();	  //UCOSII的方式,禁止调度，防止打断us延时
#endif
}

//us级延时时,恢复任务调度
void delay_osschedunlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD //使用UCOSIII
	OS_ERR err;
	OSSchedUnlock(&err); //UCOSIII的方式,恢复调度
#else					 //否则UCOSII
	OSSchedUnlock();  //UCOSII的方式,恢复调度
#endif
}

//调用OS自带的延时函数延时
//ticks:延时的节拍数
void delay_ostimedly(uint32_t ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err;
	OSTimeDly(ticks, OS_OPT_TIME_PERIODIC, &err); //UCOSIII延时采用周期模式
#else
	OSTimeDly(ticks); //UCOSII延时
#endif
}

//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{
	if (delay_osrunning == 1) //OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter(); //进入中断
		OSTimeTick(); //调用ucos的时钟服务程序
		OSIntExit();  //触发任务切换软中断
	}
}
#endif

//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
#if SYSTEM_SUPPORT_OS //如果需要支持OS.
	uint32_t reload;
#endif
	fac_us = CS_getMCLK() / 1000000;		 //系统时钟
#if SYSTEM_SUPPORT_OS						 //如果需要支持OS.
	reload = SystemCoreClock / 8000000;		 //每秒钟的计数次数 单位为K
	reload *= 1000000 / delay_ostickspersec; //根据delay_ostickspersec设定溢出时间
											 //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右
	fac_ms = 1000 / delay_ostickspersec;	 //代表OS可以延时的最少单位

	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //开启SYSTICK中断
	SysTick->LOAD = reload;					   //每1/delay_ostickspersec秒中断一次
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //开启SYSTICK
#endif
	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; //内部时钟源
}

#if SYSTEM_SUPPORT_OS //如果需要支持OS.
//延时nus
//nus为要延时的us数.
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;			 //需要的节拍数
	tcnt = 0;
	delay_osschedlock(); //阻止OS调度，防止打断us延时
	told = SysTick->VAL; //刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
	delay_osschedunlock(); //恢复OS调度
}
//延时nms
//nms:要延时的ms数
void delay_ms(uint16_t nms)
{
	if (delay_osrunning && delay_osintnesting == 0) //如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度)
	{
		if (nms >= fac_ms) //延时的时间大于OS的最少时间周期
		{
			delay_ostimedly(nms / fac_ms); //OS延时
		}
		nms %= fac_ms; //OS已经无法提供这么小的延时了,采用普通方式延时
	}
	delay_us((uint32_t)(nms * 1000)); //普通方式延时
}
#else //不用OS时
//延时nus
//nus为要延时的us数.
//注意nus的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nus<=0xffffff*1000000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对48M条件下,nus<=349525 (349ms)
void delay_us(uint32_t nus)
{
	uint32_t temp;
	SysTick->LOAD = nus * fac_us;			  //时间加载
	SysTick->VAL = 0x00;					  //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//关闭计数器
	SysTick->VAL = 0X00;							//清空计数器
}
//延时nms
void delay_ms(uint32_t nms)
{
	while(nms)
	{
		delay_us(1000);
		--nms;
	}
}
#endif
