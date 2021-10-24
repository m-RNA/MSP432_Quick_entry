/****************************************************/
// MSP432P401R
// 8-3 ��ʱ������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/6
/****************************************************/

/**************   ʵ��˵��   **************
 * 
 * ������
 * ��������ԭ��@ALIENTEK STM32
 * ʵ��10 ���벶��ʵ�� ������ֲ
 * �ڴ˱�ʾ��л �������ѧϰʹ��
 * 
 * ������
 * �� �öŰ��߽� P5.6 �� P1.0 ����������
 * �� ���������������Ƹߵ�ƽ����ʱ��
 * 
 * ��Ҫע�⣺
 * ����ɼ��ߵ�ƽ���ʱ��Ϊ4.19s
 * ����ʱ��ǿ�Ʊ�ǳɹ�������ߵ�ƽ��
 *
 **************   ˵������   **************/

#include "sysinit.h"
#include "usart.h"
#include "timA.h"
#include "led.h"
#include "key.h"
#include "delay.h"

int main(void)
{
    uint8_t key;
    uint32_t temp;

    SysInit();         //��3�� ʱ������
    uart_init(115200); //��7�� ��������
    LED_Init();        //��2�� GPIO���
    KEY_Init(0);       //��2�� GPIO����
    delay_init();      //��4�� �δ���ʱ
    TimA2_Cap_Init();  //��8�� ��ʱ��A ����

    printf("\r\n\r\nHello,MSP432!\r\n");
    MAP_Interrupt_enableMaster();
    while (1)
    {
        key = KEY_Scan(0);
        if (key == KEY1_PRES)
            LED_RED_On();
        else if (key == KEY2_PRES)
            LED_RED_Off();

        if (TIMA2_CAP_STA & 0X80) //�ɹ�������һ��������
        {
            temp = TIMA2_CAP_STA & 0X3F;
            temp *= 65536;                 //���ʱ���ܺ�
            temp += TIMA2_CAP_VAL;         //�õ��ܵĸߵ�ƽʱ��
            printf("HIGH:%dus\r\n", temp); //��ӡ�ܵĸߵ�ƽʱ��
            TIMA2_CAP_STA = 0;             //������һ�β���
        }
    }
}
