/****************************************************/
// MSP432P401R
// 7 Uart中断 接收OpenMV数据 参考示例
// Bilibili：m-RNA
// E-mail:m-RNA@qq.com
// 创建日期:2022/07/18
/****************************************************/

#include "sysinit.h"
#include "usart.h"
#include "led.h"
#include "tim32.h"

/*********   硬件底层    *********/

#define OPENMV_UART EUSCI_A2_BASE    // OpenMV使用串口
#define OPENMV_Uart_RX_LENGTH_MAX 50 // OpenMV UART最大接收长度

static uint8_t OpenMV_Uart_Rx_Index = 0;                        // UART接收计数索引
static uint8_t OpenMV_Uart_Rx_Temp = 0;                         // UART中断接收缓存
uint8_t OpenMV_Uart_Rx_Buffer[OPENMV_Uart_RX_LENGTH_MAX] = {0}; // UART接收缓存

/*********   应用层   *********/

short OpenMV_Rx_Data[4] = {0};             // 接收OpenMV数据
uint8_t OpenMV_Rx_Data_Analysis_State = 0; // OpenMV数据解析状态

void OpenMV_Check_Data_Task(void)
{
    uint16_t i;

    if (!OpenMV_Uart_Rx_Index) // 没有数据 退出
        return;

    // 接收长度是否达到
    if (OpenMV_Uart_Rx_Index < 7) // 按需修改 （7：两个short； 5：一个short；依此类推）
        return;

    // 检查格式是否正确
    if ((OpenMV_Uart_Rx_Buffer[0] != 0x2C) && (OpenMV_Uart_Rx_Buffer[1] != 0x12) && // 帧头(两个字节)
        (OpenMV_Uart_Rx_Buffer[OpenMV_Uart_Rx_Index - 1] != 0x5B))                  // 帧尾
        goto Send_Error;

    // 检查命令数据范围是否合理
    if ((OpenMV_Uart_Rx_Index - 3) % 2)
        goto Send_Error;

    // 命令正确
    for (i = 0; i < ((OpenMV_Uart_Rx_Index - 3) >> 1); ++i)
    {
        OpenMV_Rx_Data[i] = OpenMV_Uart_Rx_Buffer[2 + (i << 1) + 1] << 8; // 高8位
        OpenMV_Rx_Data[i] |= OpenMV_Uart_Rx_Buffer[2 + (i << 1) + 0];     // 低8位
    }
    OpenMV_Rx_Data_Analysis_State = 1; // 解析状态置1

    goto Jump; // 正常流程就跳过
Send_Error:    // 解析错误
    // do something ...

    printf_uart2("?"); // 回复OpenMV "?"

    printf("[Log] OpenMv Rx Data ERROR\r\n"); // 打印解析出错
Jump:
    // 清零接收缓冲 为下一次接收准备
    OpenMV_Uart_Rx_Index = 0;
    memset(OpenMV_Uart_Rx_Buffer, 0, OPENMV_Uart_RX_LENGTH_MAX);
}

// Uart2接收中断
void EUSCIA2_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(OPENMV_UART);
    if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) //接收中断
    {
        OpenMV_Uart_Rx_Temp = MAP_UART_receiveData(OPENMV_UART); // 临时接收

        if (OpenMV_Uart_Rx_Index < OPENMV_Uart_RX_LENGTH_MAX) // 接收缓冲是否溢出
        {
            OpenMV_Uart_Rx_Buffer[OpenMV_Uart_Rx_Index] = OpenMV_Uart_Rx_Temp; // 放入缓存
            OpenMV_Uart_Rx_Index++;                                            // 索引加1
        }
    }
}

int main(void)
{
    SysInit();          // 第3讲 时钟配置
    uart0_init(115200); // 第7讲 串口配置
    uart2_init(115200); // 第7讲 串口配置(OPEN MV)

    printf("[Log] Init Completed! \r\n");
    MAP_Interrupt_enableMaster(); // 开启总中断

    while (1)
    {
        OpenMV_Check_Data_Task();

        if (OpenMV_Rx_Data_Analysis_State)
        {
            // 打印一下接收数据
            printf("[OpenMV Data] x1: %d, x2: %d\r\n", OpenMV_Rx_Data[0], OpenMV_Rx_Data[1]);

            // 清除解析完成状态 清除缓存
            OpenMV_Rx_Data_Analysis_State = 0;
            memset(OpenMV_Rx_Data, 0, sizeof(OpenMV_Rx_Data));
        }
    }
}
