# MSP432入门速成_电赛延期不掉队_Ti杯_Keil

## 介绍
2021/8/27  @BILIBILI：m-RNA  
参加电赛分秒必争，光速掌握快速移植代码的能力十分重要，为了让学过stm32的电赛人，快速迁移到MSP432平台，我趁着电赛延期，尝试制作这门速成课[[MSP432入门速成]](https://www.bilibili.com/video/BV1Rb4y1z7KJ)，但由于我一人能力有限，难免会有疏漏，还请大家多多指正包涵。使用了MSP432P401R作为实验板。

## 注意
★UP所用的固件库版本v3_40_01_02  
★若想与我版本一致，则可下载ti_driverlib，下载后ti_driverlib文件夹与RNA_xx文件夹同一目录。  
★若不需要，则注意将历程RNA_xx文件夹与官方历程放与同一目录。但打开工程时，Libary下的msp432p4xx_driverlib.lib会提示找不到，这时我们移除它，再重新添加，比如我的路径在 D:\ti\simplelink_msp432p4_sdk_3_40_01_02\source\ti\devices\msp432p4xx\driverlib\keil\msp432p4xx_driverlib.lib  
找不到应该时文件类型没选对，选择Toochain Library file（*.lib）或者 All files就可以找到了。

## 视频章节
###1 开发环境
电赛人对keil熟 故讲keil的配置 还补充了VScode 直接用CCS也可  
安装包在百度网盘[[点击链接]](https://pan.baidu.com/s/10Wg93SwzNaGChqZna_vXQA) 提取码:6666

###2GPIO
####2-1 GPIO输出  
[RNA_LED]  跑马灯  
####2-2 GPIO输入  
[RNA_KEY]     按键      
[RNA_KEY4x4]  矩阵键盘
####2-3 GPIO复用  
[]  复用输出内部时钟

###3 时钟
####3-1 时钟简介
####3-2 配置外部晶振  
[RNA_CS_LFXT_HFXT]  高速晶振跑到48MHz 低速晶振跑到32768Hz 

###4 滴答计时器延时 
[RNA_SysTick_delay]  滴答计时器延时（移植正点原子） 

###5 外部中断  
[RNA_External_Interrupt]  外部中断  

###6中断调度
####6-1 中断优先级管理  
[RNA_Interrupt_Priority]  

####6-2 软件挂起中断  
[interrupt_software_pending]  官方例程  

####6-3 屏蔽中断优先级  
[interrupt_priority_masking]  官方例程 

###7 串口配置  
[RNA_UART_Int]  使用带串口的外部模块可参考
[RNA_UART]      printf("HELLO,WORLD");

###8定时器A
####8-1 定时器A中断  
[RNA_TimA_Int]

####8-2 定时器A 输出PWM  
[RNA_TimA_PWM_Square_Wave]  输出50Hz方波  
[RNA_TimA_PWM_Servo]        驱动舵机

