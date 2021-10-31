
#ifndef _bsp_ads1115_H
#define _bsp_ads1115_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* -----ADS1115 Pin ------- */
#define ads1115_I2C_SCL GPIO_PIN5
#define ads1115_I2C_SDA GPIO_PIN6
#define GPIOX_ads1115_I2C GPIO_PORT_P8

/* --------ads1115_I2C --------*/
#define ads1115_I2C_SCL_H MAP_GPIO_setOutputHighOnPin(GPIOX_ads1115_I2C, ads1115_I2C_SCL)
#define ads1115_I2C_SCL_L MAP_GPIO_setOutputLowOnPin(GPIOX_ads1115_I2C, ads1115_I2C_SCL)

#define ads1115_I2C_SDA_H MAP_GPIO_setOutputHighOnPin(GPIOX_ads1115_I2C, ads1115_I2C_SDA)
#define ads1115_I2C_SDA_L MAP_GPIO_setOutputLowOnPin(GPIOX_ads1115_I2C, ads1115_I2C_SDA)

/*=========================================================================
    ads1115_I2C ADDRESS/BITS
    -----------------------------------------------------------------------*/
#define ADS1015_ADDRESS (0x90) // 1001 000 (ADDR = GND)
                               /*=========================================================================*/

/*=========================================================================
    CONVERSION DELAY (in mS)
    -----------------------------------------------------------------------*/
#define ADS1115_CONVERSIONDELAY (8)
/*=========================================================================*/

/*=========================================================================
    POINTER REGISTER -- ֻд
    -----------------------------------------------------------------------*/
#define ADS1015_REG_POINTER_MASK (0x03)
#define ADS1015_REG_POINTER_CONVERT (0x00)   // ת���Ĵ���
#define ADS1015_REG_POINTER_CONFIG (0x01)    // ���üĴ���
#define ADS1015_REG_POINTER_LOWTHRESH (0x02) // LOWTHRESH�Ĵ���
#define ADS1015_REG_POINTER_HITHRESH (0x03)  // HITHRESH�Ĵ���
                                             /*=========================================================================*/

/*=========================================================================
    CONFIG REGISTER -- ��д
    -----------------------------------------------------------------------*/
// ����״̬/����ת����ʼ
#define ADS1015_REG_CONFIG_OS_MASK (0x8000)
#define ADS1015_REG_CONFIG_OS_SINGLE (0x8000)  // Write: ����״̬/����ת����ʼ
#define ADS1015_REG_CONFIG_OS_BUSY (0x0000)    // Read: Bit = 0 �豸Ŀǰ����ִ��ת��
#define ADS1015_REG_CONFIG_OS_NOTBUSY (0x8000) // Read: Bit = 1 �豸Ŀǰ����ִ��ת��

// �����·����������
#define ADS1015_REG_CONFIG_MUX_MASK (0x7000)
#define ADS1015_REG_CONFIG_MUX_DIFF_0_1 (0x0000) // AIN P = AIN0 �� AIN N = AIN1 (default)
#define ADS1015_REG_CONFIG_MUX_DIFF_0_3 (0x1000) // AIN P = AIN0 �� AIN N = AIN3
#define ADS1015_REG_CONFIG_MUX_DIFF_1_3 (0x2000) // AIN P = AIN1 �� AIN N = AIN3
#define ADS1015_REG_CONFIG_MUX_DIFF_2_3 (0x3000) // AIN P = AIN2 �� AIN N = AIN3
#define ADS1015_REG_CONFIG_MUX_SINGLE_0 (0x4000) // AIN P = AIN0 �� AIN N = GND
#define ADS1015_REG_CONFIG_MUX_SINGLE_1 (0x5000) // AIN P = AIN1 �� AIN N = GND
#define ADS1015_REG_CONFIG_MUX_SINGLE_2 (0x6000) // AIN P = AIN2 �� AIN N = GND
#define ADS1015_REG_CONFIG_MUX_SINGLE_3 (0x7000) // AIN P = AIN3 �� AIN N = GND

// �ɱ������Ŵ�������
#define ADS1015_REG_CONFIG_PGA_MASK (0x0E00)
#define ADS1015_REG_CONFIG_PGA_6_144V (0x0000) // +/-6.144V range = Gain 2/3
#define ADS1015_REG_CONFIG_PGA_4_096V (0x0200) // +/-4.096V range = Gain 1
#define ADS1015_REG_CONFIG_PGA_2_048V (0x0400) // +/-2.048V range = Gain 2 (default)
#define ADS1015_REG_CONFIG_PGA_1_024V (0x0600) // +/-1.024V range = Gain 4
#define ADS1015_REG_CONFIG_PGA_0_512V (0x0800) // +/-0.512V range = Gain 8
#define ADS1015_REG_CONFIG_PGA_0_256V (0x0A00) // +/-0.256V range = Gain 16

// ����ģʽ
#define ADS1015_REG_CONFIG_MODE_MASK (0x0100)
#define ADS1015_REG_CONFIG_MODE_CONTIN (0x0000) // ����ת��ģʽ
#define ADS1015_REG_CONFIG_MODE_SINGLE (0x0100) // ���絥��ģʽ(default)

// ��������
#define ADS1015_REG_CONFIG_DR_MASK (0x00E0)
#define ADS1015_REG_CONFIG_DR_128SPS (0x0000)  // 128 samples per second
#define ADS1015_REG_CONFIG_DR_250SPS (0x0020)  // 250 samples per second
#define ADS1015_REG_CONFIG_DR_490SPS (0x0040)  // 490 samples per second
#define ADS1015_REG_CONFIG_DR_920SPS (0x0060)  // 920 samples per second
#define ADS1015_REG_CONFIG_DR_1600SPS (0x0080) // 1600 samples per second (default)
#define ADS1015_REG_CONFIG_DR_2400SPS (0x00A0) // 2400 samples per second
#define ADS1015_REG_CONFIG_DR_3300SPS (0x00C0) // 3300 samples per second

// �Ƚ���ģʽ
#define ADS1015_REG_CONFIG_CMODE_MASK (0x0010)
#define ADS1015_REG_CONFIG_CMODE_TRAD (0x0000)   // ���г��͵Ĵ�ͳ�Ƚ��� (default)
#define ADS1015_REG_CONFIG_CMODE_WINDOW (0x0010) // ���ڱȽ���

// �Ƚ�������
#define ADS1015_REG_CONFIG_CPOL_MASK (0x0008)
#define ADS1015_REG_CONFIG_CPOL_ACTVLOW (0x0000) // ALERT/RDY �͵�ƽ��Ч (default)
#define ADS1015_REG_CONFIG_CPOL_ACTVHI (0x0008)  // ALERT/RDY �ߵ�ƽ��Ч

// ����Ƚ���
#define ADS1015_REG_CONFIG_CLAT_MASK (0x0004)   // ��λ������ALERT/RDY һ�������Ƿ���λ
#define ADS1015_REG_CONFIG_CLAT_NONLAT (0x0000) // ������Ƚ��� (default)
#define ADS1015_REG_CONFIG_CLAT_LATCH (0x0004)  // ����Ƚ���

// �Ƚ������кͽ���
#define ADS1015_REG_CONFIG_CQUE_MASK (0x0003)
#define ADS1015_REG_CONFIG_CQUE_1CONV (0x0000) // ALERT/RDY ��һ��ת��������
#define ADS1015_REG_CONFIG_CQUE_2CONV (0x0001) // ALERT/RDY ��һ��ת��������
#define ADS1015_REG_CONFIG_CQUE_4CONV (0x0002) // ALERT/RDY ��һ��ת��������
#define ADS1015_REG_CONFIG_CQUE_NONE (0x0003)  // ALERT/RDY ���ñȽ���(default)
/*=========================================================================*/
typedef enum
{
  GAIN_TWOTHIRDS = ADS1015_REG_CONFIG_PGA_6_144V, // 0.1875mV
  GAIN_ONE = ADS1015_REG_CONFIG_PGA_4_096V,       // 0.125mV
  GAIN_TWO = ADS1015_REG_CONFIG_PGA_2_048V,       // 0.0625mV
  GAIN_FOUR = ADS1015_REG_CONFIG_PGA_1_024V,      // 0.03125mV
  GAIN_EIGHT = ADS1015_REG_CONFIG_PGA_0_512V,     // 0.015625mV
  GAIN_SIXTEEN = ADS1015_REG_CONFIG_PGA_0_256V    //  0.0078125mV
} adsGain_t;

#define CMD_ADS1115_ADDR 0x90      /* ADS1115���豸��ַ,��ǰ��д */
#define CMD_ADS1115_Write_REG 0x90 /* д ADS1115�豸*/
#define CMD_ADS1115_Read_REG 0x91  /* �� ADS1115�豸 */

/* public: */
void ads1115_Init(void);
void SetGain(adsGain_t gain);
adsGain_t GetGain(void);
uint16_t GetAds1115Values(void);
/* privata: */
uint8_t ConfigeRegister(unsigned char channel);
uint8_t PointRegister(uint8_t ads1115_I2CAddress, uint8_t reg);
uint8_t writeRegister(uint8_t ads1115_I2CAddress, uint8_t reg, uint16_t value);
uint16_t readConvertRegister(void);

/* ����ȫ�ֺ��� */
void ads1115_I2C_INIT(void);
void ads1115_I2C_SDA_OUT(void);
void ads1115_I2C_SDA_IN(void);
void ads1115_I2C_Start(void);
void ads1115_I2C_Stop(void);
void ads1115_I2C_Ack(void);
void ads1115_I2C_NAck(void);
uint8_t ads1115_I2C_Wait_Ack(void);
void ads1115_I2C_Send_Byte(uint8_t txd);
uint8_t ads1115_I2C_Read_Byte(uint8_t ack);

/* ----- delay -------*/
void ads1115_delay_us(uint32_t i);
void ads1115_delay_ms(uint32_t i);

#endif
