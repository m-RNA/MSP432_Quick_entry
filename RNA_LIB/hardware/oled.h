#ifndef __OLED_H
#define __OLED_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define OLED_CMD 0  //写命令
#define OLED_DATA 1 //写数据

#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //置1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //置0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //获取电平
//----------------------------------------------------------------------------------
//OLED SSD1306 SPI  时钟D0  P6.0

#define OLED_SSD1306_SCL_PIN_NUM (BIT0)

#define OLED_SSD1306_SCL_IO_INIT (Set_Bit(P6DIR, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Set() (Set_Bit(P6OUT, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Clr() (Clr_Bit(P6OUT, OLED_SSD1306_SCL_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 SPI 数据D1   P6.1

#define OLED_SSD1306_SDA_PIN_NUM (BIT1)

#define OLED_SSD1306_SDA_IO_INIT (Set_Bit(P6DIR, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Set() (Set_Bit(P6OUT, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Clr() (Clr_Bit(P6OUT, OLED_SSD1306_SDA_PIN_NUM))

//----------------------------------------------------------------------------------
////OLED SSD1306 复位/RES

//#define		OLED_SSD1306_RES_PIN_NUM		(BIT2)

//#define		OLED_SSD1306_RES_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_RES_PIN_NUM))
//#define		OLED_RES_Set()				(Set_Bit(P5OUT,OLED_SSD1306_RES_PIN_NUM))
//#define		OLED_RES_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_RES_PIN_NUM))

//----------------------------------------------------------------------------------

//OLED控制用函数
void OLED_delay_ms(unsigned int ms);
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
