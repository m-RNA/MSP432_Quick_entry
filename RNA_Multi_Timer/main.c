/****************************************************/
// MSP432P401R
// 9* �����չ��ʱ�������濪�����£�
// ��Ŀ��ַ��https://github.com/0x1abin/MultiTimer
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/13
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "led.h"
#include "tim32.h"
#include "MultiTimer.h"

MultiTimer timer1;
MultiTimer timer2;
MultiTimer timer3;
MultiTimer timer4;
MultiTimer timer5;

void exampleTimer1Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%d] Timer1 callback-> %s.\r\n", MultiTimerGetValue(), (char *)userData);
}

void exampleTimer2Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%d] Timer2 callback-> %s.\r\n", MultiTimerGetValue(), (char *)userData);
    MultiTimerStart(timer, 5000, exampleTimer2Callback, userData);
}

void exampleTimer3Callback(MultiTimer *timer, void *userData)
{
    printf("[T:%d] Timer3 callback-> %s.\r\n", MultiTimerGetValue(), (char *)userData);
    MultiTimerStart(timer, 4567, exampleTimer3Callback, userData);
}

void exampleTimer4Callback(MultiTimer *timer, void *userData)
{
    LED_G_Tog();
    MultiTimerStart(&timer5, 1500, exampleTimer4Callback, NULL);
}

void delay_ms_Demo_BreathLight_BaseOnMultiTime(void)
{
    static bool dir = 1;
    static char temp = 0;
    char i;
    if (dir)
        ++temp;
    else
        --temp;
    if (temp == 14)
        dir = 0;
    else if (temp == 0)
        dir = 1;
    for (i = 0; i < 14 - temp; ++i) //�����ѭ�������ú����ȵ����������
    {
        delay_ms(temp);
        LED_RED_On();
        delay_ms(14 - temp);
        LED_RED_Off();
    }
}

int main(void)
{
    SysInit();         // ��3�� ʱ������
    uart_init(115200); // ��7�� ��������
    LED_Init();        // ��2�� GPIO���
    MultiTimerInit();  // ��10�� �����ʱ��

    MultiTimerStart(&timer1, 1000, exampleTimer1Callback, "1000ms ONCE timer");                      //�����ж�
    MultiTimerStart(&timer2, 5000, exampleTimer2Callback, "5000ms CYCLE timer");                     //ѭ���ж�
    MultiTimerStart(&timer3, 3456, exampleTimer3Callback, "3456ms delay start, 4567ms CYCLE timer"); //��ͬ���ж�
    MultiTimerStart(&timer4, 0, exampleTimer4Callback, NULL);                                        //�����ж�

    MAP_Interrupt_enableMaster(); // �������ж�
    while (1)
    {
        MultiTimerYield();
        delay_ms_Demo_BreathLight_BaseOnMultiTime();
    }
}
