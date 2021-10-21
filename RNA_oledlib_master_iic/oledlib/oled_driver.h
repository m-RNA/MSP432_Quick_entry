#ifndef __OLED_DRIVER_H
#define	__OLED_DRIVER_H
#include "sysinit.h"
#include "oled_config.h"
#include "oled_basic.h"


#if (TRANSFER_METHOD ==HW_IIC)

	#if (USE_HW_IIC ==IIC_0)
		#define OLED_ADDRESS	0x3C  //ͨ������0R����,������0x78��0x7A������ַ -- Ĭ��0x78
		/*STM32F103C8T6оƬ��Ӳ��I2C: PB6 -- SCL; PB7 -- SDA */
		#define IIC_GPIOX               GPIO_PORT_P1
		#define IIC_SCL_Pin				GPIO_PIN7
		#define IIC_SDA_Pin       		GPIO_PIN6
		#define EUSCI_BX                EUSCI_B0_BASE
	#elif (USE_HW_IIC ==IIC_2)

	#endif
	
#elif (TRANSFER_METHOD ==SW_IIC)

#define Set_Bit(val, bitn) (val |= (/*1 <<*/ (bitn))) //��1
#define Clr_Bit(val, bitn) (val &= ~(/*1<<*/ (bitn))) //��0
#define Get_Bit(val, bitn) (val & (1 << (bitn)))      //��ȡ��ƽ
//----------------------------------------------------------------------------------
//OLED SSD1306 SPI  ʱ��D0  P6.0

#define OLED_SSD1306_SCL_PIN_NUM (BIT7)

#define OLED_SSD1306_SCL_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Set() (Set_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))
#define OLED_SCL_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SCL_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 SPI ����D1   P6.1

#define OLED_SSD1306_SDA_PIN_NUM (BIT6)

#define OLED_SSD1306_SDA_IO_INIT (Set_Bit(P1DIR, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Set() (Set_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))
#define OLED_SDA_Clr() (Clr_Bit(P1OUT, OLED_SSD1306_SDA_PIN_NUM))


#elif (TRANSFER_METHOD ==HW_SPI)
	
	#if (USE_HW_SPI==SPI_1)
		
		
		
	#elif (USE_HW_SPI==SPI_2)
		#define SPIX                      SPI2
		//ʹ��SPI2ʱ��
		#define SPI_RCC_APB1Periph_SPIX   RCC_APB1Periph_SPI2
		//ʹ��gpioʱ�ӣ�ʹ�õ�GPIO��һ��ʱ�ɶ������£�
		//#define SPI_RCC_APB2Periph_GPIOX	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC
		#define SPI_RCC_APB2Periph_GPIOX	RCC_APB2Periph_GPIOB	
		//CSƬѡ�����Ƭѡ��
		#define SPI_CS_Pin_X              GPIO_Pin_12
		#define SPI_CS_GPIOX							GPIOB
		//SPI2 ʱ�ӡ�mosi��miso����
		#define SPI_HW_ALL_PINS          	(GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15) 
		#define SPI_HW_ALL_GPIOX					GPIOB
		//��λ����
		#define SPI_RES_PIN 							GPIO_Pin_10
		#define SPI_RES_GPIOX							GPIOB
		//��������
		#define SPI_DC_PIN 								GPIO_Pin_11
		#define SPI_DC_GPIOX							GPIOB
	#endif

#elif (TRANSFER_METHOD ==SW_SPI)


#endif

void I2C_Configuration(void);
void I2C_SW_Configuration(void);
void I2C_WriteByte(uint8_t addr,uint8_t data);
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
