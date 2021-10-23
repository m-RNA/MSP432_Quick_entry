#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H
#include "sysinit.h"
#include "oled_config.h"
#include "oled_basic.h"

#if (TRANSFER_METHOD == HW_IIC)
#define OLED_ADDRESS 0x3C //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78  0x3C = 0x78 >> 1

#if (USE_HW_IIC == IIC_0) // 已验证
#define IIC_GPIOX GPIO_PORT_P1
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B0_BASE

#elif (USE_HW_IIC == IIC_1) // 已验证
#define IIC_GPIOX GPIO_PORT_P6
#define IIC_SCL_Pin GPIO_PIN5
#define IIC_SDA_Pin GPIO_PIN4
#define EUSCI_BX EUSCI_B1_BASE

#elif (USE_HW_IIC == IIC_2) // 已验证
#define IIC_GPIOX GPIO_PORT_P3
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B2_BASE

#elif (USE_HW_IIC == IIC_3) // 已验证
#define IIC_GPIOX GPIO_PORT_P10
#define IIC_SCL_Pin GPIO_PIN3
#define IIC_SDA_Pin GPIO_PIN2
#define EUSCI_BX EUSCI_B3_BASE
#endif

#elif (TRANSFER_METHOD == SW_IIC)

#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //置1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //置0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //获取电平
//----------------------------------------------------------------------------------
//OLED SSD1306 SCL  P1.7

#define OLED_SSD1306_SCL_PIN_NUM (BIT7)
#define OLED_SSD1306_SCL_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Set() (Set_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))

//OLED SSD1306 SDA   P1.6
#define OLED_SSD1306_SDA_PIN_NUM (BIT6)
#define OLED_SSD1306_SDA_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Set() (Set_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))

#elif (TRANSFER_METHOD == HW_SPI)

#if (USE_HW_SPI == SPI_1)

#elif (USE_HW_SPI == SPI_2)
#define SPIX SPI2
//使能SPI2时钟
#define SPI_RCC_APB1Periph_SPIX RCC_APB1Periph_SPI2
//使能gpio时钟，使用的GPIO不一样时可定义如下：
//#define SPI_RCC_APB2Periph_GPIOX	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC
#define SPI_RCC_APB2Periph_GPIOX RCC_APB2Periph_GPIOB
//CS片选（软件片选）
#define SPI_CS_Pin_X GPIO_Pin_12
#define SPI_CS_GPIOX GPIOB
//SPI2 时钟、mosi、miso引脚
#define SPI_HW_ALL_PINS (GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15)
#define SPI_HW_ALL_GPIOX GPIOB
//复位引脚
#define SPI_RES_PIN GPIO_Pin_10
#define SPI_RES_GPIOX GPIOB
//控制引脚
#define SPI_DC_PIN GPIO_Pin_11
#define SPI_DC_GPIOX GPIOB
#endif

#elif (TRANSFER_METHOD == SW_SPI)

#endif

void I2C_Configuration(void);
void I2C_SW_Configuration(void);
void I2C_WriteByte(uint8_t addr, uint8_t data);
void SPI_Configuration(void);
void SPI_WriterByte(unsigned char dat);
void WriteCmd(unsigned char cmd);
void WriteDat(unsigned char Dat);
void OLED_Init(void);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_FILL(unsigned char BMP[]);

#endif //__OLED_DRIVER_H
