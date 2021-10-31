/****************************************************/
//MSP432P401R
//板载按键驱动(MultiButton版)
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/10/29
/****************************************************/

#include "key.h"
#include "MultiTimer.h"
#include "usart.h"

/***********************   Button Stick  ********(****************/

MultiTimer buttonTimer;
void ButtonTimerCallback(MultiTimer *timer, void *userData)
{
    button_ticks();
    MultiTimerStart(timer, 5, ButtonTimerCallback, NULL);
}

/****************************  Base  ****************************/

struct Button btn1_OnBoard;
struct Button btn2_OnBoard;

uint8_t read_button1_OnBoard_GPIO()
{
    return MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1);
}
uint8_t read_button2_OnBoard_GPIO()
{
    return MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4);
}

/************************  按键初始化函数  ***************************/

void KEY_Init(void)
{
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1 + GPIO_PIN4);

    button_init(&btn1_OnBoard, read_button1_OnBoard_GPIO, 0);
    button_init(&btn2_OnBoard, read_button2_OnBoard_GPIO, 0);

    button_start(&btn1_OnBoard);
    button_start(&btn2_OnBoard);

    MultiTimerStart(&buttonTimer, 5, ButtonTimerCallback, NULL);
}
