#include "MultiTimer.h"
#include <stdio.h>
#include "tim32.h"

#define MULTIMER_MAX_TIMEOUT  0x7fffffff

/* Check if timer's expiry time is greater than time and care about uint32_t wraparounds */
#define CHECK_TIME_LESS_THAN(t, compare_to) ( (((uint32_t)((t)-(compare_to))) > MULTIMER_MAX_TIMEOUT) ? 1 : 0 )

/*
It means 1 tick for 1ms. 
Your can configurate for your tick time such as 5ms/10ms and so on.
*/
#define CFG_TIMER_1_TICK_N_MS   1

//Timer ticks
//static uint32_t _timer_ticks = (1 << 32)- 1000; // only for test tick clock overflow
static uint32_t _multi_timer_ticks = 0;

/**
  * @brief  background ticks, timer repeat invoking interval nms.
  * @param  None.
  * @retval None.
  */
void MultiTimerTicks(void)
{
    _multi_timer_ticks += CFG_TIMER_1_TICK_N_MS;
}

uint32_t platformTicksFunction(void)
{
    return _multi_timer_ticks;
}

/* Timer handle list head. */
static MultiTimer* timerList = NULL;

#define ARR 47999                  // 自动重装载值
#define CLKDIV TIMER32_PRESCALER_1 // 时钟源分频
/*
 * 定时器中断周期：
 *
 * T_timer_32 = CLKDIV * (ARR + 1) / f_clk 
 *            = 1 * 48000 / 48000000 
 *            = 1ms = 1000Hz
 */

void MultiTimerInit(void)
{
	Tim32_1_Int_Init(ARR,CLKDIV);
}

int MultiTimerStart(MultiTimer* timer, uint32_t timing, MultiTimerCallback_t callback, void* userData)
{
    if (!timer || !callback || timing > MULTIMER_MAX_TIMEOUT / 2 ) {
        return -1;
    }
    MultiTimer** nextTimer = &timerList;
    /* Remove the existing target timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        if (timer == *nextTimer) {
            *nextTimer = timer->next; /* remove from list */
            break;
        }
    }

    /* Init timer. */
    timer->deadline = platformTicksFunction() + timing;
    timer->callback = callback;
    timer->userData = userData;

    /* Insert timer. */
    for (nextTimer = &timerList;; nextTimer = &(*nextTimer)->next) {
        if (!*nextTimer) {
            timer->next = NULL;
            *nextTimer = timer;
            break;
        }
        if (timer->deadline < (*nextTimer)->deadline) {
            timer->next = *nextTimer;
            *nextTimer = timer;
            break;
        }
    }
    return 0;
}

int MultiTimerStop(MultiTimer* timer)
{
    MultiTimer** nextTimer = &timerList;
    /* Find and remove timer. */
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        MultiTimer* entry = *nextTimer;
        if (entry == timer) {
            *nextTimer = timer->next;
            break;
        }
    }
    return 0;
}

int MultiTimerYield(void)
{
    MultiTimer** nextTimer = &timerList;
    for (; *nextTimer; nextTimer = &(*nextTimer)->next) {
        MultiTimer* entry = *nextTimer;
        /* Sorted list, just process with the front part. */
        if (CHECK_TIME_LESS_THAN(platformTicksFunction(), entry->deadline)) {
            return (int)(entry->deadline - platformTicksFunction());
        }
        /* remove expired timer from list */
        *nextTimer = entry->next;

        /* call callback */
        if (entry->callback) {
            entry->callback(entry, entry->userData);
        }
        if (entry->next == NULL) {
            return 0;
        }
    }
    return 0;
}
