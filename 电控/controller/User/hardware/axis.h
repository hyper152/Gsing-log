/*
 * axis.h
 *
 *  Created on: Sep 4, 2025
 *      Author: ASUS
 */

#ifndef AXIS_H_
#define AXIS_H_

#include "adc.h"
#include "gpio.h"

extern uint16_t adc[6];
extern uint8_t L[2];
extern uint8_t R[2];
extern uint8_t Round[2];
void Axis_init();
void Axis_read();




#endif /* AXIS_H_ */
