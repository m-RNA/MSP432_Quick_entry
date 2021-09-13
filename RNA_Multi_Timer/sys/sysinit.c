#include "sysinit.h"
/****************************************************/
//MSP432P401R
//ʱ������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
/****************************************************/

//High:48MHz  Low:32768Hz
//MCLK=48MHz  SMCLK=48MHz
void SysInit(void)
{
    WDTCTL = WDTPW | WDTHOLD; // ͣ�ÿ��Ź�

    /* ��һ����Ҫ�������ǵ�ʱ�����ţ�����ĸ���ʱ��ʹ�õ����ⲿ����*/
    //����ʱ�ӳ�ʼ���Ƚ���
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN3 | GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION); //High
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ, GPIO_PIN0 | GPIO_PIN1, GPIO_PRIMARY_MODULE_FUNCTION); //Low
    CS_setExternalClockSourceFrequency(32768, 48000000);

    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* �������������ʹ�õĵȴ�״̬�����ڶ�ȡ������
    ���ı�ʱ�ӵ�Ƶ�ʷ�Χʱ������ʹ�ô˺���������ɶ�����
    ͨ����������CPU��̫���ˣ�Flash�����ϣ���CPU�ȵ��� */
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 1);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 1);

    CS_startHFXT(false);          //���Ǿ��� ��Ҫ����
    CS_startLFXT(CS_LFXT_DRIVE3); //�����ȼ�3

    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  //48MHz   16��Ƶʱ���δ���ʱ�ɴﵽ�
    MAP_CS_initClockSignal(CS_SMCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1); //48MHz
}
