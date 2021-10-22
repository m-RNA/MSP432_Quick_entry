/******************************************************************************
//MSP432P401R
//10 OLEDͼ�ο⣨Ӳ��IIC/ģ��IIC��
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/9/17
*******************************************************************************/

/*********  ����  **********
 *
 * 0.96��OLED
 * UP��OLEDͼ�ο�����
 * ��ֲ����MSP432������
 * ��Ŀ��ַ��https://github.com/hello-myj/stm32_oled
 
 * ֧��ģ��IIC��Ӳ��IIC
 * ���Žӷ�:
 * P1.7 SCL
 * P1.6 SDA
 * ����ȥͷ�ļ���������
 *
 ****************************/

#include "sysinit.h"
#include "delay.h"
#include "test.h"
#include "draw_api.h"

int main(void)
{
    SysInit();
    delay_init();
    InitGraph();
    delay_ms(200);
    ClearScreen(); //����
    while (1)
    {
        demo();
        ZJY_Demo(); //OLED_TEST�������
    }
}
