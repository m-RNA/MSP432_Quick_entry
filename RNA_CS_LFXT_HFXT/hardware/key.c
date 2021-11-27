/****************************************************/
//MSP432P401R
//板载按键驱动
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

#include "key.h"

//函数功能：延时
void key_delay(uint16_t t);

//按键初始化函数
//mode:0,不开启中断;1,开启中断
void KEY_Init(bool mode) //IO初始化
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);

    if (mode)
    {
        /* Configuring P1.1 P1.4 as an input and enabling interrupts */
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1); //清理中断标志
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4); //清理中断标志
        MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);    //使能中断端口
        MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);    //使能中断端口
        MAP_Interrupt_enableInterrupt(INT_PORT1);             //端口总使能

        /* Enabling SRAM Bank Retention */
        MAP_SysCtl_enableSRAMBankRetention(SYSCTL_SRAM_BANK1);
    }
}

/* GPIO 中断 */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1); //获取中断状态
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);         //清理中断标志
    key_delay(25);                                             //去抖动

    if (status & GPIO_PIN1) //对应P1.1
    {
        if (KEY1 == 0)
        {
            /*开始填充用户代码*/

            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);

            /*结束填充用户代码*/
        }
    }
    if (status & GPIO_PIN4) //对应P1.4
    {
        if (KEY2 == 0)
        {
            /*开始填充用户代码*/

            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);

            /*结束填充用户代码*/
        }
    }
}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//注意此函数有响应优先级,KEY1>KEY2!!
uint8_t KEY_Scan(bool mode)
{
    static bool key_up = true; //按键按松开标志
    if (mode)
        key_up = true; //支持连按
    if (key_up && (KEY1 == 0 || KEY2 == 0))
    {
        key_delay(25); //去抖动
        key_up = false;
        if (KEY1 == 0)
            return KEY1_PRES;
        else if (KEY2 == 0)
            return KEY2_PRES;
    }
    else if (KEY1 == 1 && KEY2 == 1)
        key_up = true;
    return 0; // 无按键按下
}

//函数功能：延时
static void key_delay(uint16_t t)
{
    volatile uint16_t x;
    while (t--)
        for (x = 0; x < 1000; x++)
            ;
}
