/*******************************************
//MSP432P401R
//4 移植滴答延时
//Bilibili：m-RNA
//E-mail:m-RNA@qq.com
//创建日期:2021/7/22
*******************************************/

#ifndef __DELAY_H
#define __DELAY_H
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

  /**
  * @brief  Delay Init Function.
  * @param  Delay Init.
  * @retval None.
  */
  void delay_init(void);

  /**
  * @brief  Delay ms Function.
  * @param  Delay ms.
  * @retval None.
  */
  void delay_ms(uint32_t ms);

  /**
  * @brief  Delay us Function.
  * @param  Delay us.
  * @retval None.
  */
  void delay_us(uint32_t nus);

#endif
