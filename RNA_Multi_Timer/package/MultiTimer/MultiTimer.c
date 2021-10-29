#include "MultiTimer.h"
#include <stdio.h>

#define MULTIMER_MAX_TIMEOUT 0x7fffffff

/* Check if timer's expiry time is greater than time and care about uint32_t wraparounds */
#define CHECK_TIME_LESS_THAN(t, compare_to) ((((uint32_t)((t) - (compare_to))) > MULTIMER_MAX_TIMEOUT) ? 1 : 0)

/*
It means 1 tick for 1ms. 
Your can configurate for your tick time such as 5ms/10ms and so on.
*/
#define CFG_TIMER_1_TICK_N_MS 1

//Timer ticks
//static uint32_t _timer_ticks = (1 << 32)- 1000; // only for test tick clock overflow
volatile static uint32_t _multi_timer_ticks = 0;

/**
  * @brief  background ticks, timer repeat invoking interval nms.
  * @param  None.
  * @retval None.
  */
void MultiTimerTicks(void)
{
    _multi_timer_ticks += CFG_TIMER_1_TICK_N_MS;
}

uint32_t MultiTimerGetValue(void)
{
    return _multi_timer_ticks;
}

void delay_init(void);
/* Timer handle list head. */
static MultiTimer *timerList = NULL;

void MultiTimerInit(void)
{
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(MAP_CS_getMCLK() / 1000);
    MAP_SysTick_enableInterrupt();
    delay_init();
}

void SysTick_Handler(void)
{
    MultiTimerTicks();
}

int MultiTimerStart(MultiTimer *timer, uint32_t timing, MultiTimerCallback_t callback, void *userData)
{
    if (!timer || !callback || timing > MULTIMER_MAX_TIMEOUT / 2)
    {
        return -1;
    }
    MultiTimer **nextTimer = &timerList;
    /* Remove the existing target timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next)
    {
        if (timer == *nextTimer)
        {
            *nextTimer = timer->next; /* remove from list */
            break;
        }
    }

    /* Init timer. */
    timer->deadline = MultiTimerGetValue() + timing;
    timer->callback = callback;
    timer->userData = userData;

    /* Insert timer. */
    for (nextTimer = &timerList;; nextTimer = &(*nextTimer)->next)
    {
        if (!*nextTimer)
        {
            timer->next = NULL;
            *nextTimer = timer;
            break;
        }
        if (timer->deadline < (*nextTimer)->deadline)
        {
            timer->next = *nextTimer;
            *nextTimer = timer;
            break;
        }
    }
    return 0;
}

int MultiTimerStop(MultiTimer *timer)
{
    MultiTimer **nextTimer = &timerList;
    /* Find and remove timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next)
    {
        MultiTimer *entry = *nextTimer;
        if (entry == timer)
        {
            *nextTimer = timer->next;
            break;
        }
    }
    return 0;
}

int MultiTimerYield(void)
{
    MultiTimer **nextTimer = &timerList;
    for (; *nextTimer; nextTimer = &(*nextTimer)->next)
    {
        MultiTimer *entry = *nextTimer;
        /* Sorted list, just process with the front part. */
        if (CHECK_TIME_LESS_THAN(MultiTimerGetValue(), entry->deadline))
        {
            return (int)(entry->deadline - MultiTimerGetValue());
        }
        /* remove expired timer from list */
        *nextTimer = entry->next;

        /* call callback */
        if (entry->callback)
        {
            entry->callback(entry, entry->userData);
        }
        if (entry->next == NULL)
        {
            return 0;
        }
    }
    return 0;
}
/************************  Delay  *********************************/

MultiTimer delayTimer;
static uint32_t _delayTimer_ms = 0;
static uint8_t fac_us = 48;

static void delay_init(void)
{
    fac_us = CS_getMCLK() / 1000000; //系统时钟
    delay_ms(0);
}

void DelayTimer1Callback(MultiTimer *timer, void *userData)
{
    volatile uint32_t temp = _multi_timer_ticks + _delayTimer_ms;
    while (_multi_timer_ticks < temp)
        MultiTimerYield();
}

void delay_ms(uint32_t ms)
{
    _delayTimer_ms = ms;
    MultiTimerStart(&delayTimer, 0, DelayTimer1Callback, NULL);
    //MultiTimerYield(); // 这里是加好呢，还是不加好呢。。。。。
}

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD; //LOAD的值
    ticks = nus * fac_us;            //需要的节拍数
    tcnt = 0;
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
    }
}
