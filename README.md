# 【电赛救急】MSP432入门速成 Keil/CCS 

## 简要介绍
2021/8/27  [@BILIBILI：m-RNA](https://space.bilibili.com/41224928  "@BILIBILI：m-RNA 个人主页")    
参加电赛分秒必争，光速掌握快速移植代码的能力十分重要，为了让学过stm32的电赛人，快速迁移到MSP432平台，我趁着电赛延期，尝试制作这门速成课[[Bilibili：MSP432入门速成]](https://www.bilibili.com/video/BV1Rb4y1z7KJ "Bilibili: MSP432入门速成")，PPT、脑图和安装包在百度网盘 https://pan.baidu.com/s/10Wg93SwzNaGChqZna_vXQA  提取码:6666。但由于我一人能力有限，难免会有疏漏，还请大家多多指正包涵。E-mail：m-RNA@qq.com 使用了MSP432P401R作为实验板。

其他想说的：

1. 本项目完全免费，不要被骗money💴了呀朋友
2. 本代码基于Apache-2.0开源，开源您的代码时，有引用或者修改本代码的地方，请标明作者与出处。尊重作者，尊重开源~  
3. 仓库地址  

      - GitHub链接：https://github.com/m-RNA/MSP432_Quick_entry  

      - Gitee链接：https://gitee.com/chenjjian/MSP432_Quick_entry

## 编译报错
1. RNA_Emtpy是空工程，此工程完全独立，已将驱动打包至此工程（支持Keil跟CCS平台开发）

2. 以后不知官方的固件库还会不会改，编译报错请自行对比，这里UP所用的固件库版本v3_40_01_02

3. 若想与作者固件库版本一致，则可下载ti_driverlib，下载后ti_driverlib文件夹与RNA_xx文件夹同一目录；若不需要，则注意将历程RNA_xx文件夹与官方历程放与同一目录。但打开工程时，Libary下的msp432p4xx_driverlib.lib会提示找不到，这时我们移除它，再重新添加，比如我的路径在 :

   > D:\ti\simplelink_msp432p4_sdk_3_40_01_02\source\ti\devices\msp432p4xx\driverlib\keil\msp432p4xx_driverlib.lib

   找不到应该时文件类型没选对，选择 Toochain Library file（*.lib）或者 All files 就可以找到了。  其他文件如果出现类似情况也是同理，先移除，再添加。   
   


## 工程简介
​        一般电赛人对Keil比较熟，故Keil工程全配置，当然还配置一部分CCS工程。Keil相关和CCS安装包在百度网盘[[点击链接]](https://pan.baidu.com/s/10Wg93SwzNaGChqZna_vXQA) 提取码:6666

| 历程名称                          | 具体用途                           | 包含CCS工程 | 包含Keil工程 |
| --------------------------------- | ---------------------------------- | ----------- | ------------ |
| RNA_LED                           | 跑马灯                             |             | ✔            |
| RNA_KEY                           | 板载按键                           |             | ✔            |
| RNA_KEY4x4                        | 矩阵键盘                           |             | ✔            |
| RNA_KEY(MultiButton_Poll)         | 按键（MultiButton）                | ✔           | ✔            |
| RNA_KEY(MultiButton_Callback)     | 按键（MultiButton）                | ✔           | ✔            |
| RNA_CS_LFXT_HFXT                  | 配置外部晶振为48M                  |             | ✔            |
| RNA_SysTick_delay                 | 滴答计时器延时（移植）             |             | ✔            |
| RNA_External_Interrupt            | 演示按键外部中断                   |             | ✔            |
| RNA_Interrupt_Priority            | 中断优先级调度管理                 |             | ✔            |
| RNA_UART                          | printf("HELLO,WORLD");             | ✔           | ✔            |
| RNA_UART_Int                      | 串口中断（模块可参考）             |             | ✔            |
| RNA_TimA_Int                      | 定时器A中断                        | ✔           | ✔            |
| RNA_TimA_PWM_Servo                | 驱动舵机                           | ✔           | ✔            |
| RNA_TimA_PWM_Multi_Servo          | 多路PWM输出(舵机)                  | ✔           | ✔            |
| RNA_TimA_PWM_Square_Wave          | 输出50Hz方波                       | ✔           | ✔            |
| RNA_TimA_CAP_Key_Press_Time       | 捕获计算脉冲时长（移植）           | ✔           | ✔            |
| RNA_Tim32_Int                     | 定时器32中断                       |             | ✔            |
| RNA_Multi_Timer                   | 软件无限扩展定时器(移植)           | ✔           | ✔            |
| RNA_Multi_Timer_Old               | 原作者2020年介绍版本               |             | ✔            |
| RNA_OLED_Test_IIC                 | OLED显示测试(IIC)                  | ✔           | ✔            |
| RNA_OLED_LIB_IIC                  | OLED图像库(IIC)                    | ✔           | ✔            |
| RNA_ADC_Multi_Channel             | ADC多路采集                        | ✔           | ✔            |
| RNA_Empty(traditional)            | 传统开发(正点原子风格)             | ⭕           | ✔            |
| RNA_Empty(MultiTimer_MultiButton) | 添加软件包(MultiTimer+MultiButton) | ⭕           | ✔            |

使用了以下开源工程：

- [[MultiButton]](https://github.com/0x1abin/MultiButton  "@GitHub： MultiButton")

- [[MultiTimer]](https://github.com/0x1abin/MultiTimer  "@GitHub： MultiTimer")

- [[STM32 OLED图形库]](https://github.com/hello-myj/stm32_oled  "@GitHub： STM32 OLED图形库")

## 致谢名单
- 感谢B站UP主[“CloudBoyStudio”](https://space.bilibili.com/72364842 )一直无私奉献，耐心细致解答问题。本工程中很多代码，比如ADC的DMA传输配置等，参考了UP主“CloudBoyStudio”在的MSP432学习交流群发出的示例代码，在此深表感谢😆
- 感谢B站UP主“物联世界”视频教程；
- 感谢正点原子在Stm32平台的贡献，使得我们方便将代码移植到MSP432平台上来；
- 感谢B站、开源社区，让我发现了很多有趣且实用的项目，带给我很多欢乐、知识；
- ......