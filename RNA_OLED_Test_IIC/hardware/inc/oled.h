#ifndef __OLED_H
#define __OLED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define HW_IIC 0
#define SW_IIC 1
#define HW_SPI 2
#define SW_SPI 3

#define IIC_1 10
#define IIC_2 11
#define IIC_3 12
#define IIC_4 13

/*****************************************************************/
//配置驱动
//1.HW_IIC  硬件IIC
//2.SW_IIC  软件IIC
//3.HW_SPI	硬件SPI
//4.SW_SPI	软件IIC

//现在只支持 硬件IIC 和 软件IIC
#define TRANSFER_METHOD HW_IIC

/*****************************************************************/

#if (TRANSFER_METHOD == HW_IIC)

//IIC_0: P1.7 -- SCL; P1.6 -- SDA
//IIC_1: P6.5 -- SCL; P6.4 -- SDA
//IIC_2: P3.7 -- SCL; P3.6 -- SDA
#define USE_HW_IIC IIC_2

#elif (TRANSFER_METHOD == HW_SPI)
//#define USE_HW_SPI SPI_2
#endif

#if (TRANSFER_METHOD == HW_IIC)
#define OLED_ADDRESS 0x3C //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78  0x3C = 0x78 >> 1

#if (USE_HW_IIC == IIC_0)
#define IIC_GPIOX GPIO_PORT_P1
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B0_BASE

#elif (USE_HW_IIC == IIC_1)
// 未验证
#define IIC_GPIOX GPIO_PORT_P6
#define IIC_SCL_Pin GPIO_PIN5
#define IIC_SDA_Pin GPIO_PIN4
#define EUSCI_BX EUSCI_B1_BASE

#elif (USE_HW_IIC == IIC_2)
// 未验证
#define IIC_GPIOX GPIO_PORT_P3
#define IIC_SCL_Pin GPIO_PIN7
#define IIC_SDA_Pin GPIO_PIN6
#define EUSCI_BX EUSCI_B2_BASE

#elif (USE_HW_IIC == IIC_3)
// ????
// #define IIC_GPIOX GPIO_PORT_P10
// #define IIC_SCL_Pin GPIO_PIN3
// #define IIC_SDA_Pin GPIO_PIN2
// #define EUSCI_BX EUSCI_B3_BASE
#endif

#elif TRANSFER_METHOD == SW_IIC
#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //置1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //置0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //获取电平

//OLED SSD1306 I2C SCL P1.7
#define OLED_SSD1306_SCL_PIN_NUM (BIT7)
#define OLED_SSD1306_SCL_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Set() (Set_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))

//OLED SSD1306 I2C SDA   P1.6
#define OLED_SSD1306_SDA_PIN_NUM (BIT6)
#define OLED_SSD1306_SDA_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Set() (Set_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))

#elif (TRANSFER_METHOD == HW_SPI)
//暂未支持
#endif

//OLED控制用函数
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat, uint8_t cmd);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey);
uint32_t oled_pow(uint8_t m, uint8_t n);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey);
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[]);
void OLED_Init(void);
#endif
