/****************************************************/
// MSP432P401R
// 第8讲 定时器A 三路SPWM输出
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2021/8/26
/****************************************************/

/*******************     SPWM x3      ****************

    定时器说明：
	T0  中断  调整定时器TimA1输出的PWM波为SPWM
	T1  PWM	 引脚占用 P7.5  P7.6  P7.7

    按键说明：
    KEY1 改变频率
    KEY2 改变幅值

******************************************************/

#include "sysinit.h"
#include "usart.h"
#include "delay.h"
#include "timA.h"
#include "key.h"
#include "led.h"

int main(void)
{
    volatile uint8_t i = 0;
    volatile uint8_t key = 0; // 键值

    SysInit();         // 第3讲 时钟配置
    uart_init(115200); // 第7讲 串口配置
    delay_init();      // 第4讲 滴答延时
    LED_Init();        // 第2讲 GPIO输出
    KEY_Init(0);       // 第2讲 GPIO输入
    SpwmInit();        // 第8讲 SPWM输出

    /* 打印初始信息 */
    printf("Hello MSP423!\r\n");
    printf("Frequency:%dHz\r\n", FundamentalFrequency);
    printf("MaxDutyCycle:%f\r\n", MaxDutyCycle);

    MAP_Interrupt_enableMaster();
    while (1)
    {
        key = KEY_Scan(0); // 扫描按键
        if (key == 1)      // 按下KEY1改变输出基波的频率
        {
            // 限制频率为 25~100Hz
            if (FundamentalFrequency == 100) //
            {
                FundamentalFrequency = 25;
            }
            else
            {
                FundamentalFrequency += 1;
            }
            // 改变定时器A0的控制频率，来改变输出的SPWM的基波频率
            MAP_Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0, 48000000 / (SamplesPerCycle * FundamentalFrequency));
            printf("Frequency:%dHz\r\n", FundamentalFrequency);
        }
        else if (key == 2) // 按下KEY1改变输出基波的幅值
        {
            // 限制最大占空比为 50~95%
            if (MaxDutyCycle == 0.95f)
            {
                MaxDutyCycle = 0.5f;
            }
            else
            {
                MaxDutyCycle += 0.05f;
            }
            printf("MaxDutyCycle:%f\r\n", MaxDutyCycle);
        }
        key = 0;
        delay_ms(20);

        /* 闪烁板载红灯来示意工作中 */
        if (i == 10)
        {
            i = 0;
            LED_RED_Tog(); // 闪烁红灯
        }
        else
        {
            ++i;
        }
    }
}
