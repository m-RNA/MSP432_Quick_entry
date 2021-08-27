#include "sysinit.h"
#include "uartinit.h"
#include "led.h"

/******************************************************************************
// MSP432P401R
// 2-2-3 GPIO���� ��Ӿ������
// Bilibili��m-RNA
// E-mail:m-RNA@qq.com
// ��������:2021/8/11
*******************************************************************************/

int main(void)
{
    char a[100];
    SysInit();         //��3�� ʱ������
    uart_init(115200); //��7�� ����ʵ��
    LED_Init();        //��2�� GPIO���

    printf("MSP432\r\n");
    printf("2021/8/24\r\n");

    LED_RED_On();
    while (1)
    {
        scanf("%s", a);
        printf("%s\r\n", a);

        // MAP_PCM_gotoLPM0();
    }
}
