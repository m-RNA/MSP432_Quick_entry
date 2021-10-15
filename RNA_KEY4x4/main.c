/******************************************************************************
//MSP432P401R
//2-2-3 GPIO���� ��Ӿ������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
*******************************************************************************/

#include "sysinit.h"
#include "uartinit.h"
#include "key4x4.h"

int main(void)
{
    uint8_t key_value;

    SysInit();         //��3�� ʱ������
    uart_init(115200); //��7�� ����ʵ��
    KEY4x4_Init();     //��2�� GPIO����
	
    printf("MSP432\r\n");
    printf("2021/8/11\r\n");
	
    while (1)
    {
        key_value = KEY4x4_Scan(0);
        if (key_value)
            printf("%d", key_value);
    }
}
