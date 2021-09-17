#include "uartinit.h"

int fputc(int ch, FILE *f)
{
    UART_transmitData(EUSCI_A0_BASE, ch & 0xFF);
    return ch;
}

int fgetc(FILE *f)
{
    while (EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG !=
           UART_getInterruptStatus(
               EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG))
        ;
    return UART_receiveData(EUSCI_A0_BASE);
}

//SMCLK 48MHz   比特率 115200
const eUSCI_UART_ConfigV1 uartConfig =
    {
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,                // SMCLK Clock Source
        26,                                            // BRDIV = 26
        0,                                             // UCxBRF = 0
        111,                                           // UCxBRS = 111
        EUSCI_A_UART_NO_PARITY,                        // No Parity
        EUSCI_A_UART_LSB_FIRST,                        // MSB First
        EUSCI_A_UART_ONE_STOP_BIT,                     // One stop bit
        EUSCI_A_UART_MODE,                             // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
        EUSCI_A_UART_8_BIT_LEN                         // 8 bit data length
};


//SMCLK 48MHz   比特率 115200
void UartInit(void)
{
    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);
    /* Enable UART module */
    MAP_UART_enableModule(EUSCI_A0_BASE);

    // /***  配置中断 Enabling interrupts  ***/
    // MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    // MAP_Interrupt_enableInterrupt(INT_EUSCIA2);
    // MAP_Interrupt_enableSleepOnIsrExit();

    //这个是啥？？？我不知道
    //我从历程抄的
    // uint8_t TXData = 1;
    // uint8_t RXData = 0;
    // while (1)
    // {
    //     MAP_UART_transmitData(EUSCI_A0_BASE, TXData);
    //     MAP_Interrupt_enableSleepOnIsrExit();
    //     MAP_PCM_gotoLPM0InterruptSafe();
    // }
}

// /* EUSCI A0 UART ISR - Echos data back to PC host */
// void EUSCIA2_IRQHandler(void)
// {
//     uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

//     if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
//     {
//         RXData = MAP_UART_receiveData(EUSCI_A0_BASE);

//         if (RXData != TXData) // Check value
//         {
//             MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);
//             while (1)
//                 ; // Trap CPU
//         }
//         TXData++;
//         MAP_Interrupt_disableSleepOnIsrExit();
//     }
// }
