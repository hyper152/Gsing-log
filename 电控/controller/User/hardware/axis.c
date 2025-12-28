/*
 * axis.c
 *
 *  Created on: Sep 4, 2025
 *      Author: ASUS
 */

#include"axis.h"
uint16_t adc[6];//前四个是摇杆，后两个是旋钮
uint8_t L[2];
uint8_t R[2];
uint8_t Round[2];


void Axis_init(){
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc, sizeof(adc)/sizeof(uint16_t));
}



void Axis_read(){


	L[0] = (adc[0]-350)/(36.2-3.5);
	L[1] = (adc[1]-300)/(37.3-3);
	R[0] = (adc[2]-420)/(35.5-4.2);
	R[1] = (adc[3]-330)/(36.5-3.3);
	Round[0] =  adc[4]/40.5;
	Round[1] =  adc[5]/40.5;
	HAL_Delay(1);


}
