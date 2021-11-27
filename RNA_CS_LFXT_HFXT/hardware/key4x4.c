/****************************************************/
//MSP432
//外接矩阵键盘驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

/************          接法如下          ************/
/*                                                    
 *         Rows and columns  行和列
 *
 * R4   R3   R2   R1     C1   C2   C3   C4
 * P4.0 P4.1 P4.2 P4.3   P4.4 P4.5 P4.6 P4.7
 *
 */
/*****************************************************/

#include "key4x4.h"

//函数功能：延时
void key_delay(uint16_t t);

void KEY4x4_Init(void)
{
    P4DIR &= ~(BIT0 | BIT1 | BIT2 | BIT3); // P4.0~3 设为输入
    P4REN |= BIT0 | BIT1 | BIT2 | BIT3;    // P4.0~3 使能上拉/下拉电阻
    P4OUT |= BIT0 | BIT1 | BIT2 | BIT3;    // P4.0~3 上拉
    P4DIR |= BIT4 | BIT5 | BIT6 | BIT7;    // P4.4~7 设为输出
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按; 1,支持连续按
uint8_t KEY4x4_Scan(bool mode)
{
    uint8_t i;
    static bool key_up = true; //按键按松开标志
    if (mode)
        key_up = true; //支持连按
    if ((~(P4IN | 0xf0)) && key_up)
    {
        key_delay(25); //去抖动
        for (i = 0; i < 4; i++)
        {
            key_up = false;
            P4OUT = ~(1 << (i + 4));      //+4是应为 忽略低四位
            switch (~P4IN & 0x0F) //第i+1行
            {
            case 0x08: //第四列
                return i + 1;
            case 0x04: //第三列
                return i + 5;
            case 0x02: //第二列
                return i + 9;
            case 0x01: //第一列
                return i + 13;
            }
        }
    }
    else if ((P4IN & 0x01) && (P4IN & 0x02) && (P4IN & 0x04) && (P4IN & 0x08))
        key_up = true;
    return 0;
}

//函数功能：延时
static void key_delay(uint16_t t)
{
    volatile uint16_t x;
    while (t--)
        for (x = 0; x < 1000; x++)
            ;
}
