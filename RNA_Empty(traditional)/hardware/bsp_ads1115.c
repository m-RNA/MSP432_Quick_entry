#include "bsp_ads1115.h"
#include "usart.h"

/* 增益 */
adsGain_t m_gain;
/* 测试通道 */
uint8_t m_channel = 0;

/**
  * @brief  设置配置寄存器
  * @param  None
  * @retval None:
  */
void ads1115_Init(void)
{
  ads1115_I2C_INIT(); // ads1115_I2C init

  /* 增益 */
  m_gain = GAIN_ONE;

  /* config the config reg */
  if (ConfigeRegister(m_channel))
  {
    // deal error
    printf("init configreg error\r\n");
  }
}

/**
  * @brief  设置配置寄存器
  * @param  channel : 通道X(0..3)
  * @retval None:
  */
uint8_t ConfigeRegister(unsigned char channel)
{

  /* Set config reg valus */
  uint16_t config = ADS1015_REG_CONFIG_CQUE_NONE |    // 禁用比较器              (default val)
                    ADS1015_REG_CONFIG_CLAT_NONLAT |  // 非锁存比较器            (default val)
                    ADS1015_REG_CONFIG_CPOL_ACTVLOW | // ALERT/RDY 低电平有效    (default val)
                    ADS1015_REG_CONFIG_CMODE_TRAD |   // 具有迟滞的传统比较器     (default val)
                    ADS1015_REG_CONFIG_DR_1600SPS |   // 1600 samples per second (default val)
                    ADS1015_REG_CONFIG_MODE_CONTIN;   // Single-shot mode

  /* Set PGA/voltage range */
  config |= m_gain;

  /* Set 'start single-conversion' bit */
  config |= ADS1015_REG_CONFIG_OS_SINGLE;

  /* Set single-ended input channel */
  switch (channel)
  {
  case (0):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_0;
    break;
  case (1):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_1;
    break;
  case (2):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_2;
    break;
  case (3):
    config |= ADS1015_REG_CONFIG_MUX_SINGLE_3;
    break;
  }

  /* Write config register to the ADC */
  if (writeRegister(CMD_ADS1115_Write_REG, ADS1015_REG_POINTER_CONFIG, config))
  {

    // deal no ack error
    return 1;
  }

  /* normal return */
  return 0;
}

/**
  * @brief  指针寄存器
  * @param  ads1115_I2CAddress : ADS115地址和读写位 0bit控制读写
  * @param  reg        :  要操作的寄存器
  * @retval None:
  */
uint8_t PointRegister(uint8_t ads1115_I2CAddress, uint8_t reg)
{

  /* ads1115_I2C start */
  ads1115_I2C_Start();

  /* sent addr and write reg */
  ads1115_I2C_Send_Byte(ads1115_I2CAddress);
  /* ack */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  /* Point reg is seted convert reg */
  ads1115_I2C_Send_Byte(reg);
  /* ack */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  /* ads1115_I2C stop */
  ads1115_I2C_Stop();

  /* normal rerturn */
  return 0;
}

/**
  * @brief  设置增益
  * @param  gain : 增益 
  * @retval None:
  */
void SetGain(adsGain_t gain)
{
  m_gain = gain;
}

/**
  * @brief  取得增益
  * @param  None
  * @retval None
  */
adsGain_t GetGain(void)
{
  return m_gain;
}

/**
  * @brief  写寄存器
  * @param  ads1115_I2CAddress : ADS115地址和读写位 0bit控制读写
  * @param  reg        :  要操作的寄存器
  * @param  value      :  写入寄存器的值
  * @retval None
  */
uint8_t writeRegister(uint8_t ads1115_I2CAddress, uint8_t reg, uint16_t value)
{

  ads1115_I2C_Start();
  /* 地址和读写地址 */
  ads1115_I2C_Send_Byte(ads1115_I2CAddress);
  /* ACK */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  /* 寄存器地址 */
  ads1115_I2C_Send_Byte(reg);
  /* ACK */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  /* Lo_thresh寄存器 */
  ads1115_I2C_Send_Byte((uint8_t)(value >> 8));
  /* ACK */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  /* Hi_thresh寄存器 */
  ads1115_I2C_Send_Byte((uint8_t)(value & 0xFF));
  /* ACK */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }
  ads1115_I2C_Stop();

  /* normal return */
  return 0;
}

/**
  * @brief  读取convert寄存器
  * @param  None
  * @retval None
  */
uint16_t readConvertRegister(void)
{
  uint8_t high = 0;
  uint8_t low = 0;
  /* ads1115_I2C start */
  ads1115_I2C_Start();
  /* addr and read reg */
  ads1115_I2C_Send_Byte((uint8_t)CMD_ADS1115_Read_REG);
  /* ACK */
  if (ads1115_I2C_Wait_Ack())
  {
    return 1;
  }

  high = ads1115_I2C_Read_Byte(1);
  low = ads1115_I2C_Read_Byte(1);

  /* ads1115_I2C stop */
  ads1115_I2C_Stop();

  return (high << 8 | low);
}

/**
  * @brief  获得adc的值
  * @param  None
  * @retval None
  */
uint16_t GetAds1115Values(void)
{
  /* point to convert reg .ready to read adc values */
  if (PointRegister(CMD_ADS1115_Write_REG, ADS1015_REG_POINTER_CONVERT))
  {
    // deal error
    printf("init PointRegister error\r\n");
  }

  return readConvertRegister();
}

/* ------------- ads1115_I2C drive ---------- */

/**
  * @brief  IIC初始化IO
  * @param  None
  * @retval None
  */
void ads1115_I2C_INIT()
{
  MAP_GPIO_setAsOutputPin(GPIOX_ads1115_I2C, ads1115_I2C_SCL | ads1115_I2C_SDA);

  ads1115_I2C_SCL_H;
  ads1115_I2C_SDA_H;
}

/**
  * @brief  SDA输出配置
  * @param  None
  * @retval None
  */
void ads1115_I2C_SDA_OUT()
{
  MAP_GPIO_setAsOutputPin(GPIOX_ads1115_I2C, ads1115_I2C_SDA);
}

/**
  * @brief  SDA输入配置
  * @param  None
  * @retval None
  */
void ads1115_I2C_SDA_IN(void)
{
    MAP_GPIO_setAsInputPin(GPIOX_ads1115_I2C,ads1115_I2C_SDA);

}

/**
  * @brief  产生起始信号信号
  * @param  None
  * @retval None

SCL   -------|_____


SDA    ---|________

*/
void ads1115_I2C_Start(void)
{
  ads1115_I2C_SDA_OUT();

  ads1115_I2C_SDA_H;
  ads1115_I2C_SCL_H;
  ads1115_delay_us(2);
  ads1115_I2C_SDA_L;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_L;
}

/**
  * @brief  产生停止信号
  * @param  None
  * @retval None

SCL   ------------

SDA   ____|-------
  
*/
void ads1115_I2C_Stop(void)
{
  ads1115_I2C_SDA_OUT();

  ads1115_I2C_SCL_L;
  ads1115_I2C_SDA_L;
  ads1115_I2C_SCL_H;
  ads1115_delay_us(2);
  ads1115_I2C_SDA_H;
  ads1115_delay_us(2);
}

/**
  * @brief  主机产生应答信号ACK
  * @param  None
  * @retval None
  */
void ads1115_I2C_Ack(void)
{
  ads1115_I2C_SCL_L;
  ads1115_I2C_SDA_OUT();
  ads1115_I2C_SDA_L;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_H;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_L;
}

/**
  * @brief  主机不产生应答信号ACK
  * @param  None
  * @retval None
  */
void ads1115_I2C_NAck(void)
{
  ads1115_I2C_SCL_L;
  ads1115_I2C_SDA_OUT();
  ads1115_I2C_SDA_H;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_H;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_L;
}

/**
  * @brief  等待从机应答信号
  * @param  txd 要发送的字符
  * @retval 返回值：1 接收应答失败 0 接收应答成功
有ACK:
SCL   -------

SDA   ---|____

没有ACK;

SCL   -------

SDA   -------
*/
uint8_t ads1115_I2C_Wait_Ack(void)
{
  uint8_t tempTime = 0;

  ads1115_I2C_SDA_IN();

  ads1115_I2C_SDA_H;
  ads1115_delay_us(2);
  ads1115_I2C_SCL_H;
  ads1115_delay_us(2);

  while (MAP_GPIO_getInputPinValue(GPIOX_ads1115_I2C, ads1115_I2C_SDA))
  {
    tempTime++;
    if (tempTime > 10)
    {
      // ads1115_I2C_Stop();
      return 1;
    }
  }

  ads1115_I2C_SCL_L;
  return 0;
}
//ads1115_I2C 发送一个字节

/**
  * @brief  ads1115_I2C 发送一个字节
  * @param  txd 要发送的字符
  * @retval None:
  *
 SCL __|----|___|-----|___

 SDA __|--------|_________

数据:  |-- 1----| ----0---|

*/
void ads1115_I2C_Send_Byte(uint8_t txd)
{
  uint8_t i = 0;

  ads1115_I2C_SDA_OUT();
  ads1115_I2C_SCL_L; //拉低时钟开始数据传输

  for (i = 0; i < 8; i++)
  {
    if ((txd & 0x80) > 0) //0x80  1000 0000
      ads1115_I2C_SDA_H;
    else
      ads1115_I2C_SDA_L;

    txd <<= 1;
    ads1115_I2C_SCL_H;
    ads1115_delay_us(2); //发送数据
    ads1115_I2C_SCL_L;
    ads1115_delay_us(2);
  }
}

/**
  * @brief  ads1115_I2C 读取一个字节
  * @param  ask: 1 读取有应答,0 没有应答
  * @retval None:
  */
uint8_t ads1115_I2C_Read_Byte(uint8_t ack)
{
  uint8_t i = 0, receive = 0;

  ads1115_I2C_SDA_IN();
  for (i = 0; i < 8; i++)
  {
    ads1115_I2C_SCL_L;
    ads1115_delay_us(2);
    ads1115_I2C_SCL_H;
    receive <<= 1;
    if (MAP_GPIO_getInputPinValue(GPIOX_ads1115_I2C, ads1115_I2C_SDA))
      receive++;
    ads1115_delay_us(1);
  }

  if (ack == 0)
    ads1115_I2C_NAck();
  else
    ads1115_I2C_Ack();

  return receive;
}

/* -------------- delay --------------*/

/**
  * @brief  延时函数，延时us
  * @param  i(0..255)
  * @retval None:
  */
void ads1115_delay_us(uint32_t i)
{
  uint32_t temp;
  SysTick->LOAD = 9 * i; //设置重装数值, 72MHZ时
  SysTick->CTRL = 0X01;  //使能，减到零是无动作，采用外部时钟源
  SysTick->VAL = 0;      //清零计数器
  do
  {
    temp = SysTick->CTRL;                           //读取当前倒计数值
  } while ((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
  SysTick->CTRL = 0;                                //关闭计数器
  SysTick->VAL = 0;                                 //清空计数器
}

/**
  * @brief  延时函数，延时ms
  * @param  i(0..255)
  * @retval None:
  */
void ads1115_delay_ms(uint32_t i)
{
  uint32_t temp;
  SysTick->LOAD = 9000 * i; //设置重装数值, 72MHZ时
  SysTick->CTRL = 0X01;     //使能，减到零是无动作，采用外部时钟源
  SysTick->VAL = 0;         //清零计数器
  do
  {
    temp = SysTick->CTRL;                           //读取当前倒计数值
  } while ((temp & 0x01) && (!(temp & (1 << 16)))); //等待时间到达
  SysTick->CTRL = 0;                                //关闭计数器
  SysTick->VAL = 0;                                 //清空计数器
}
