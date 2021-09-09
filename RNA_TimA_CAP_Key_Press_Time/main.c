#include "sysinit.h"
#include "uartinit.h"
#include "timA.h"
#include "led.h"
#include "key.h"

/****************************************************/
// MSP432P401R
// 8-3 ��ʱ������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/9/6
/****************************************************/

/*****************   ʵ��˵��   *****************
 *
 * ���� ����ԭ��@ALIENTEK �� ʵ��10 ���벶��ʵ��
 * ������ֲ���ǳ���л��
 * 
 * ��Ҫע�⣺
 * ����Ϊ16λʱ���ɼ��ߵ�ƽ���ʱ��Ϊ4.19s��
 * ����ʱ��ǿ�Ʊ�ǳɹ�������ߵ�ƽ��
 *
 *****************   ˵������   *****************/

int main(void)
{
    uint8_t key;
    uint32_t temp;

    SysInit();         //��3�� ʱ������
    uart_init(115200); //��7�� ��������
    LED_Init();        //��2�� GPIO���
    KEY_Init();        //��2�� GPIO����

    TimA2_Cap_Init(); //��8�� ��ʱ��A ����

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
            temp *= 65536;                              //���ʱ���ܺ�
            temp += TIMA2_CAP_VAL;                      //�õ��ܵĸߵ�ƽʱ��
            printf("HIGH:%f s\r\n", temp / 1000000.0f); //��ӡ�ܵĸߵ�ƽʱ��
            TIMA2_CAP_STA = 0;                          //������һ�β���
        }
    }
}