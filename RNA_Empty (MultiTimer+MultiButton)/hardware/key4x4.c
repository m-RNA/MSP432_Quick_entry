/****************************************************/
//MSP432
//��Ӿ����������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
/****************************************************/

/************          �ӷ�����          ************/
/*                                                    
 *         Rows and columns  �к���
 *
 * R4   R3   R2   R1     C1   C2   C3   C4
 * P4.0 P4.1 P4.2 P4.3   P4.4 P4.5 P4.6 P4.7
 *
 */
/*****************************************************/

#include "key4x4.h"

//�������ܣ���ʱ
void key_delay(uint16_t t);

void KEY4x4_Init(void)
{
    P4DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); // P4.0~3 ��Ϊ����
    P4REN |= BIT0 | BIT1 | BIT2 | BIT3;    // P4.0~3 ʹ������/��������
    P4OUT |= BIT0 | BIT1 | BIT2 | BIT3;    // P4.0~3 ����
    P4DIR |= BIT4 | BIT5 | BIT6 | BIT7;    // P4.4~7 ��Ϊ���
}

//����������
//���ذ���ֵ
//mode:0,��֧��������; 1,֧��������
uint8_t KEY4x4_Scan(bool mode)
{
    uint8_t i;
    static bool key_up = true; //�������ɿ���־
    if (mode)
        key_up = true; //֧������
    if ((~(P4IN | 0xf0)) && key_up)
    {
        key_delay(25); //ȥ����
        for (i = 0; i < 4; i++)
        {
            key_up = false;
            P4OUT = ~(1 << (i + 4));      //+4��ӦΪ ���Ե���λ
            switch (~P4IN & 0x0F) //��i+1��
            {
            case 0x08: //������
                return i + 1;
            case 0x04: //������
                return i + 5;
            case 0x02: //�ڶ���
                return i + 9;
            case 0x01: //��һ��
                return i + 13;
            }
        }
    }
    else if ((P4IN & 0x01) && (P4IN & 0x02) && (P4IN & 0x04) && (P4IN & 0x08))
        key_up = true;
    return 0;
}

//�������ܣ���ʱ
static void key_delay(uint16_t t)
{
    volatile uint16_t x;
    while (t--)
        for (x = 0; x < 1000; x++)
            ;
}
