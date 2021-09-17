#include "sysinit.h"
#include "uartinit.h"
#include "delay.h"
#include "bmp.h"
#include "test.h"
#include "draw_api.h"

/******************************************************************************
//MSP432P401R
//10 OLEDͼ�ο⣨ģ��IIC��
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/9/17
*******************************************************************************/

/*********  ����  **********
 *
 * 0.96��OLED
 * UP��OLEDͼ�ο�����
 * ��ֲ����MSP432������
 * Ŀǰ����ֲ��ģ��IIC��ʽ������Ƚ��������Ƿ�������ţ�
 * ���������Ӳ��IIC��ʽ��
 * 
 * ��Ŀ��ַ��https://github.com/hello-myj/stm32_oled
 *
 * ���Žӷ�:
 * P6.0 SCL
 * P6.1 SDA
 * ����ȥͷ�ļ���������
****************************/

int main(void)
{
    SysInit();
    UartInit();
    delay_init();
    InitGraph();

    while (1)
    {
        demo();
    }
}
