/*
 * delay.h
 *
 *  Created on: 2016Äê1ÔÂ29ÈÕ
 *      Author: Mike
 */

#ifndef SYS_DELAY_H_
#define SYS_DELAY_H_

#include "stdint.h"
#include "msp432p401r.h"
#include "core_cm4.h"
#include <ti/devices/msp432p4xx/driverlib/cs.h>

//void delay_init(uint32_t SystemCoreClock);
void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint16_t nms);

#endif /* SYS_DELAY_H_ */
