/******************************************************************************
//MSP432P401R
//10 OLED��ʾ��ģ��IIC��
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/9/12
*******************************************************************************/

#include "sysinit.h"
#include "delay.h"
#include "oled.h"
#include "bmp.h"

/*********  ����  **********
 *
 * 0.96��OLED
 * UP���о�԰��MSP430����
 * ��ֲ����MSP432������
 * ����΢�޸���һ�²��Դ���
 *
 * ���Žӷ�:
 * P6.0 SCL
 * P6.1 SDA
 * ����ȥͷ�ļ���������
****************************/

int main(void)
{
    char t;

    SysInit();    // ��3��  ʱ������
    delay_init(); // ��4��  �δ���ʱ
    OLED_Init();  // ��10�� OLED��ʾ

    while (1)
    {
        //��ʾͼƬ
        OLED_DrawBMP(0, 0, 128, 64, BMP1);
        delay_ms(500);
        OLED_Clear(); //����

        //��ʾ����
        OLED_ShowChinese(0, 0, 0, 16);   //��
        OLED_ShowChinese(18, 0, 1, 16);  //��
        OLED_ShowChinese(36, 0, 2, 16);  //԰
        OLED_ShowChinese(54, 0, 3, 16);  //��
        OLED_ShowChinese(72, 0, 4, 16);  //��
        OLED_ShowChinese(90, 0, 5, 16);  //��
        OLED_ShowChinese(108, 0, 6, 16); //��

        //��ʾ�ַ���
        OLED_ShowString(8, 2, (uint8_t *)"ZHONGJINGYUAN", 16);
        OLED_ShowString(20, 4, (uint8_t *)"2014/05/01", 16);
        OLED_ShowString(0, 6, (uint8_t *)"ASCII:", 16);
        OLED_ShowString(63, 6, (uint8_t *)"CODE:", 16);

        //��ʾ�ַ�������
		for (t = ' '; t < '~'; t++)
		{
			OLED_ShowChar(48, 6, t, 16);	//��ʾASCII�ַ�
			OLED_ShowNum(103, 6, t, 3, 16); //��ʾASCII�ַ�����ֵ
		}
		
        delay_ms(500);
        OLED_Clear(); //����
    }
}
