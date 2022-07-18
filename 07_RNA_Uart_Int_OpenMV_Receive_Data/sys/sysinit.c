/****************************************************/
//MSP432P401R
//时钟配置
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/8/11
/****************************************************/

#include "sysinit.h"

//High:48MHz  Low:32768Hz
//MCLK=48MHz  SMCLK=48MHz
void SysInit(void)
{
    WDTCTL = WDTPW | WDTHOLD; // 停用看门狗

    /* 第一步需要配置我们的时钟引脚，这里的高速时钟使用的是外部晶振*/
    //低速时钟初始化比较慢
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION); //High
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION); //Low
    CS_setExternalClockSourceFrequency(32768, 48000000);

    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* 更改闪存控制器使用的等待状态数用于读取操作。
    当改变时钟的频率范围时，必须使用此函数以允许可读闪存
    通俗来讲就是CPU跑太快了，Flash跟不上，让CPU等等它 */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    CS_startHFXT(false);          //这是晶体 需要驱动
    CS_startLFXT(CS_LFXT_DRIVE3); //驱动等级3

    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  //48MHz   16分频时，滴答延时可达到最长
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); //48MHz
}
