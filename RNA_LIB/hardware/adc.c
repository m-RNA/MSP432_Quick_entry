/****************************************************/
//MSP432P401R
//ADC采集
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/9/13
/****************************************************/

/*********************
 *
 * 最大采集电压 3.3V
 *
 * ADC采集引脚：
 * 单路 为 P5.5
 * 双路 为 P5.5 P5.4
 * 三路 为 P5.5 P5.4 P5.3
 *
 ************************/
 
#include "adc.h"
#include "usart.h"
 
//总时间  M*N*21us
#define N 200 //采样次数
#define M 2   //采样通道个数

static uint16_t resultsBuffer[M];

void ADC_Config(void)
{
	/* 启用浮点运算的FPU */
    MAP_FPU_enableModule();
    MAP_FPU_enableLazyStacking();
	
    /* Initializing ADC (MCLK/1/1) */
    MAP_ADC14_enableModule();                                                                 //使能ADC14模块
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_8, ADC_NOROUTE); //初始化ADC 时钟 分频  通道 6MHz

#if M == 1
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5, GPIO_TERTIARY_MODULE_FUNCTION); //模拟输入
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);                                                    //单通道配置 多次转化true
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);       //使用内部电源电压参考 非差分输入false
    MAP_ADC14_enableInterrupt(ADC_INT0);                                                                    //ADC通道0的中断

#elif M == 2
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true); //多通道配置 多次转化true
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A1, false);
    MAP_ADC14_enableInterrupt(ADC_INT1);

#elif M == 3
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3, GPIO_TERTIARY_MODULE_FUNCTION); //
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A0, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A1, false);
    MAP_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A2, false);
    MAP_ADC14_enableInterrupt(ADC_INT2);

#endif
    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14); //ADC模块的中断
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION); //自动触发

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();        //使能开始转换(触发后 自动ADC上电)
    MAP_ADC14_toggleConversionTrigger(); //开启第一次软件触发
}

void ADC14_IRQHandler(void)
{
    uint8_t i = 0;
    uint_fast64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);
	
#if M == 1
    if (ADC_INT0 & status)
#elif M == 2
    if (ADC_INT1 & status)
#elif M == 3
    if (ADC_INT2 & status)
#endif
    {
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
        for (i = 0; i < M; i++)
            printf("[%d]:%d\r\t", i, resultsBuffer[i]);
        printf("\r\n");
    }
}
