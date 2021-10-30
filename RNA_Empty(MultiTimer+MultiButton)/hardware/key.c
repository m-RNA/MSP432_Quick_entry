/****************************************************/
//MSP432P401R
//板载按键驱动(MultiButton版)
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/10/29
/****************************************************/

#include "key.h"
#include "multi_button.h"
#include "MultiTimer.h"
#include "usart.h"

/************************  Button Stick  *****************************/

MultiTimer buttonTimer;
void ButtonTimerCallback(MultiTimer *timer, void *userData)
{
    button_ticks();
    MultiTimerStart(timer, 5, ButtonTimerCallback, NULL);
}

/************************  Base Start  *******************************/

struct Button btn1_OnBoard;
struct Button btn2_OnBoard;

uint8_t read_button1_OnBoard_GPIO()
{
    return KEY1;
}
uint8_t read_button2_OnBoard_GPIO()
{
    return KEY2;
}

/************************  Base End  *********************************/

void BTN1_OnBoard_SINGLE_Click_Handler(void *btn)
{
    printf("BTN1_OnBoard_SINGLE_Click\r\n");
}
void BTN1_OnBoard_DOUBLE_Click_Handler(void *btn)
{
    printf("BTN1_OnBoard_DOUBLE_Click\r\n");
}

void BTN2_OnBoard_SINGLE_Click_Handler(void *btn)
{
    printf("BTN2_OnBoard_SINGLE_Click\r\n");
}
void BTN2_OnBoard_DOUBLE_Click_Handler(void *btn)
{
    printf("BTN2_OnBoard_DOUBLE_Click\r\n");
}

//按键初始化函数
void KEY_Init(void) //IO初始化
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);
    button_init(&btn1_OnBoard, read_button1_OnBoard_GPIO, 0);
    button_init(&btn2_OnBoard, read_button2_OnBoard_GPIO, 0);

    button_attach(&btn1_OnBoard, SINGLE_CLICK, BTN1_OnBoard_SINGLE_Click_Handler);
    button_attach(&btn1_OnBoard, DOUBLE_CLICK, BTN1_OnBoard_DOUBLE_Click_Handler);
    button_attach(&btn2_OnBoard, SINGLE_CLICK, BTN2_OnBoard_SINGLE_Click_Handler);
    button_attach(&btn2_OnBoard, DOUBLE_CLICK, BTN2_OnBoard_DOUBLE_Click_Handler);

    button_start(&btn1_OnBoard);
    button_start(&btn2_OnBoard);

    MultiTimerStart(&buttonTimer, 5, ButtonTimerCallback, NULL);
}
