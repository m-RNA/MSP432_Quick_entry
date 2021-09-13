#include "sysinit.h"
#include "uartinit.h"
#include "led.h"
#include "tim32.h"
#include "multi_timer.h"

/****************************************************/
// MSP432P401R
// 9* ��ʱ��32 ���������չ��ʱ����master��
// ��Ŀ��ַ��https://github.com/0x1abin/MultiTimer
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/13
/****************************************************/

/*
 * ��ʱ���ж����ڣ�
 *
 * T_timer_32 = CLKDIV * (ARR + 1) / f_clk
 *            = 1 * 48000 / 48000000
 *            = 1ms = 1000Hz
 */

#define CLKDIV TIMER32_PRESCALER_1 // ʱ��Դ��Ƶ
#define ARR 47999                  // �Զ���װ��ֵ

struct Timer timer1;
struct Timer timer2;
struct Timer timer3;

void timer1_callback(void *arg)
{
    LED_RED_Tog();
}

void timer2_callback(void *arg)
{
    printf("timer2 timeout! arg: %p\r\n", arg);
}

void timer3_callback(void *arg)
{
    printf("timer3 timeout! arg: %p\r\n", arg);
}

int main(void)
{
    SysInit();                     // ��3�� ʱ������
    uart_init(115200);             // ��7�� ��������
    Tim32_1_Int_Init(ARR, CLKDIV); // ��9�� TIM32�ж�
	
    LED_Init();

    timer_init(&timer1, timer1_callback, 50, 50, NULL);     //  50ms ѭ��
    timer_init(&timer2, timer2_callback, 1000, 1000, NULL); //    1s ѭ��
    timer_init(&timer3, timer3_callback, 1234, 0, NULL);    //1.234s һ��

    timer_start(&timer1);
    timer_start(&timer2);
    timer_start(&timer3);

    MAP_Interrupt_enableMaster(); // �������ж�

    while (1)
    {
        timer_loop();
    }
}
