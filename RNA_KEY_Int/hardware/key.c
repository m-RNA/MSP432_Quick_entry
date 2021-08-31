#include "key.h"
#include "led.h"

/****************************************************/
//MSP432P401R
//���ذ�������
//Bilibili��m-RNA
//E-mail:m-RNA@qq.com
//��������:2021/8/11
/****************************************************/

//�������ܣ���ʱ
void key_delay(uint16_t t);

//������ʼ������
//mode:0,�������ж�;1,�����ж�
void KEY_Init(bool mode) //IO��ʼ��
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);
    if (mode)
    {
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);                               //�����жϱ�־
        MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);                               //�����жϱ�־
        MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION); //���ô�����ʽ
        MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_HIGH_TO_LOW_TRANSITION); //���ô�����ʽ
        MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);                                  //ʹ���ж϶˿�
        MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);                                  //ʹ���ж϶˿�
        MAP_Interrupt_enableInterrupt(INT_PORT1);                                           //�˿���ʹ��
    }
}

/* GPIO �ж� */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1); //��ȡ�ж�״̬
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);         //�����жϱ�־
    key_delay(5);                                              //ȥ����

    if (status & GPIO_PIN1) //��ӦP1.1
    {
        if (KEY1 == 0)
        {
            /*��ʼ����û�����*/

            LED_RED_On();

            /*��������û�����*/
        }
    }
    if (status & GPIO_PIN4) //��ӦP1.4
    {
        if (KEY2 == 0)
        {
            /*��ʼ����û�����*/

            LED_RED_Off();

            /*��������û�����*/
        }
    }
}

//����ɨ�躯��
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//ע��˺�������Ӧ���ȼ�,KEY1>KEY2!!
uint8_t KEY_Scan(bool mode)
{
    static bool key_up = true; //�������ɿ���־
    if (mode)
        key_up = true; //֧������
    if (key_up && (!KEY1 || !KEY2))
    {
        key_delay(5); //ȥ����
        key_up = false;
        if (!KEY1)
            return KEY1_PRES;
        else if (!KEY2)
            return KEY2_PRES;
    }
    else if (KEY1 && KEY2)
        key_up = true;
    return 0; // �ް�������
}

//�������ܣ���ʱ
static void key_delay(uint16_t t)
{
    volatile uint16_t x;
    while (t--)
        for (x = 0; x < 1000; x++)
            ;
}
