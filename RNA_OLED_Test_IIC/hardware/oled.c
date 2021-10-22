#include "oled.h"
#include "oledfont.h"
#include "delay.h"

#define OLED_DATA 1
#define OLED_CMD 0

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

#if (TRANSFER_METHOD == HW_IIC)
//I2C_Configuration����ʼ��Ӳ��IIC����
void I2C_Configuration(void)
{
  MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
      IIC_GPIOX, IIC_SCL_Pin | IIC_SDA_Pin, GPIO_PRIMARY_MODULE_FUNCTION);
  const eUSCI_I2C_MasterConfig i2cConfig =
      {
          EUSCI_B_I2C_CLOCKSOURCE_SMCLK,   // SMCLK Clock Source
          48000000,                        // SMCLK = 48MHz
          EUSCI_B_I2C_SET_DATA_RATE_1MBPS, // Desired I2C Clock of 1MHz��ʵ�ʿ��Ը��ߣ�����I2CЭ�����3.4MHz��ע��������������
          0,                               // No byte counter threshold
          EUSCI_B_I2C_NO_AUTO_STOP         // No Autostop
      };
  MAP_I2C_initMaster(EUSCI_B0_BASE, &i2cConfig);
  MAP_I2C_setSlaveAddress(EUSCI_B0_BASE, OLED_ADDRESS);
  MAP_I2C_setMode(EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_MODE);
  MAP_I2C_enableModule(EUSCI_B0_BASE);

  // MAP_I2C_clearInterruptFlag(
  // 	EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_NAK_INTERRUPT);
  // MAP_I2C_enableInterrupt(
  // 	EUSCI_B0_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0 | EUSCI_B_I2C_NAK_INTERRUPT);
  // MAP_Interrupt_enableInterrupt(INT_EUSCIB0);
}
//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
  if (mode)
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x40);
  else
    MAP_I2C_masterSendMultiByteStart(EUSCI_B0_BASE, 0x00);
  MAP_I2C_masterSendMultiByteFinish(EUSCI_B0_BASE, dat);
}

#elif (TRANSFER_METHOD == SW_IIC)

void I2C_SW_Configuration()
{
  OLED_SSD1306_SCL_IO_INIT;
  OLED_SSD1306_SDA_IO_INIT;
  delay_ms(200);
}

//��ʼ�ź�
void I2C_Start(void)
{
  OLED_SDA_Set();
  OLED_SCL_Set();
  OLED_SDA_Clr();
  OLED_SCL_Clr();
}

//�����ź�
void I2C_Stop(void)
{
  OLED_SDA_Clr();
  OLED_SCL_Set();
  OLED_SDA_Set();
}

//�ȴ��ź���Ӧ
void I2C_WaitAck(void) //�������źŵĵ�ƽ
{
  OLED_SDA_Set();
  OLED_SCL_Set();
  OLED_SCL_Clr();
}

//д��һ���ֽ�
void Send_Byte(uint8_t dat)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
  {
    OLED_SCL_Clr(); //��ʱ���ź�����Ϊ�͵�ƽ
    if (dat & 0x80) //��dat��8λ�����λ����д��
    {
      OLED_SDA_Set();
    }
    else
    {
      OLED_SDA_Clr();
    }

    OLED_SCL_Set();

    OLED_SCL_Clr();
    dat <<= 1;
  }
}

//����һ���ֽ�
//��SSD1306д��һ���ֽڡ�
//mode:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(uint8_t dat, uint8_t mode)
{
  I2C_Start();
  Send_Byte(0x78);
  I2C_WaitAck();
  if (mode)
  {
    Send_Byte(0x40);
  }
  else
  {
    Send_Byte(0x00);
  }
  I2C_WaitAck();
  Send_Byte(dat);
  I2C_WaitAck();
  I2C_Stop();
}
#elif (TRANSFER_METHOD == HW_SPI)
//��δ֧��
#endif

//���Ժ���
void OLED_ColorTurn(uint8_t i)
{
  if (i == 0)
  {
    OLED_WR_Byte(0xA6, OLED_CMD); //������ʾ
  }
  if (i == 1)
  {
    OLED_WR_Byte(0xA7, OLED_CMD); //��ɫ��ʾ
  }
}

//��Ļ��ת180��
void OLED_DisplayTurn(uint8_t i)
{
  if (i == 0)
  {
    OLED_WR_Byte(0xC8, OLED_CMD); //������ʾ
    OLED_WR_Byte(0xA1, OLED_CMD);
  }
  if (i == 1)
  {
    OLED_WR_Byte(0xC0, OLED_CMD); //��ת��ʾ
    OLED_WR_Byte(0xA0, OLED_CMD);
  }
}

//��������
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
  OLED_WR_Byte(0xb0 + y, OLED_CMD);
  OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
  OLED_WR_Byte((x & 0x0f), OLED_CMD);
}
//����OLED��ʾ
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
  OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
  OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC����
  OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
  OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
  uint8_t i, n;
  for (i = 0; i < 8; i++)
  {
    OLED_WR_Byte(0xb0 + i, OLED_CMD); //����ҳ��ַ��0~7��
    OLED_WR_Byte(0x00, OLED_CMD);     //������ʾλ�á��е͵�ַ
    OLED_WR_Byte(0x10, OLED_CMD);     //������ʾλ�á��иߵ�ַ
    for (n = 0; n < 128; n++)
      OLED_WR_Byte(0, OLED_DATA);
  } //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//sizey:ѡ������ 6x8  8x16
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t sizey)
{
  uint8_t c = 0, sizex = sizey / 2;
  uint16_t i = 0, size1;
  if (sizey == 8)
    size1 = 6;
  else
    size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
  c = chr - ' '; //�õ�ƫ�ƺ��ֵ
  OLED_Set_Pos(x, y);
  for (i = 0; i < size1; i++)
  {
    if (i % sizex == 0 && sizey != 8)
      OLED_Set_Pos(x, y++);
    if (sizey == 8)
      OLED_WR_Byte(asc2_0806[c][i], OLED_DATA); //6X8�ֺ�
    else if (sizey == 16)
      OLED_WR_Byte(asc2_1608[c][i], OLED_DATA); //8x16�ֺ�
    //		else if(sizey==xx) OLED_WR_Byte(asc2_xxxx[c][i],OLED_DATA);//�û������ֺ�
    else
      return;
  }
}
//m^n����
uint32_t oled_pow(uint8_t m, uint8_t n)
{
  uint32_t result = 1;
  while (n--)
    result *= m;
  return result;
}
//��ʾ����
//x,y :�������
//num:Ҫ��ʾ������
//len :���ֵ�λ��
//sizey:�����С
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t sizey)
{
  uint8_t t, temp, m = 0;
  uint8_t enshow = 0;
  if (sizey == 8)
    m = 2;
  for (t = 0; t < len; t++)
  {
    temp = (num / oled_pow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1))
    {
      if (temp == 0)
      {
        OLED_ShowChar(x + (sizey / 2 + m) * t, y, ' ', sizey);
        continue;
      }
      else
        enshow = 1;
    }
    OLED_ShowChar(x + (sizey / 2 + m) * t, y, temp + '0', sizey);
  }
}
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t sizey)
{
  uint8_t j = 0;
  while (chr[j] != '\0')
  {
    OLED_ShowChar(x, y, chr[j++], sizey);
    if (sizey == 8)
      x += 6;
    else
      x += sizey / 2;
  }
}
//��ʾ����
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t no, uint8_t sizey)
{
  uint16_t i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  for (i = 0; i < size1; i++)
  {
    if (i % sizey == 0)
      OLED_Set_Pos(x, y++);
    if (sizey == 16)
      OLED_WR_Byte(Hzk[no][i], OLED_DATA); //16x16�ֺ�
    //		else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//�û������ֺ�
    else
      return;
  }
}

//��ʾͼƬ
//x,y��ʾ����
//sizex,sizey,ͼƬ����
//BMP��Ҫ��ʾ��ͼƬ
void OLED_DrawBMP(uint8_t x, uint8_t y, uint8_t sizex, uint8_t sizey, uint8_t BMP[])
{
  uint16_t j = 0;
  uint8_t i, m;
  sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
  for (i = 0; i < sizey; i++)
  {
    OLED_Set_Pos(x, i + y);
    for (m = 0; m < sizex; m++)
    {
      OLED_WR_Byte(BMP[j++], OLED_DATA);
    }
  }
}

//��ʼ��SSD1306
void OLED_Init(void)
{
#if (TRANSFER_METHOD == HW_IIC)
  I2C_Configuration();
#elif (TRANSFER_METHOD == SW_IIC)
  I2C_SW_Configuration();
#elif (TRANSFER_METHOD == HW_SPI)
  SPI_Configuration();
#endif

  OLED_WR_Byte(0xAE, OLED_CMD); //--turn off oled panel
  OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
  OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
  OLED_WR_Byte(0x40, OLED_CMD); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81, OLED_CMD); //--set contrast control register
  OLED_WR_Byte(0xCF, OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1, OLED_CMD); //--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
  OLED_WR_Byte(0xC8, OLED_CMD); //Set COM/Row Scan Direction   0xc0���·��� 0xc8����
  OLED_WR_Byte(0xA6, OLED_CMD); //--set normal display
  OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f, OLED_CMD); //--1/64 duty
  OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00, OLED_CMD); //-not offset
  OLED_WR_Byte(0xd5, OLED_CMD); //--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80, OLED_CMD); //--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9, OLED_CMD); //--set pre-charge period
  OLED_WR_Byte(0xF1, OLED_CMD); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA, OLED_CMD); //--set com pins hardware configuration
  OLED_WR_Byte(0x12, OLED_CMD);
  OLED_WR_Byte(0xDB, OLED_CMD); //--set vcomh
  OLED_WR_Byte(0x40, OLED_CMD); //Set VCOM Deselect Level
  OLED_WR_Byte(0x20, OLED_CMD); //-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02, OLED_CMD); //
  OLED_WR_Byte(0x8D, OLED_CMD); //--set Charge Pump enable/disable
  OLED_WR_Byte(0x14, OLED_CMD); //--set(0x10) disable
  OLED_WR_Byte(0xA4, OLED_CMD); // Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6, OLED_CMD); // Disable Inverse Display On (0xa6/a7)
  OLED_Clear();
  OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}