#include "button.h"
uint8_t button[16];
void Button_read(){

	button[0] = HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
	button[1] = HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
//	button[2] = HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
	button[3] = HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
	button[4] = HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin);
	button[5] = HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin);
	button[6] = HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin);
	button[7] = HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin);
	button[8] = HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin);
	button[9] = HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin);
	button[10] = HAL_GPIO_ReadPin(KEY11_GPIO_Port, KEY11_Pin);
	button[11] = HAL_GPIO_ReadPin(KEY12_GPIO_Port, KEY12_Pin);
	button[12] = HAL_GPIO_ReadPin(KEY13_GPIO_Port, KEY13_Pin);
	button[13] = HAL_GPIO_ReadPin(KEY14_GPIO_Port, KEY14_Pin);
	button[14] = HAL_GPIO_ReadPin(KEY15_GPIO_Port, KEY15_Pin);
	button[15] = HAL_GPIO_ReadPin(KEY16_GPIO_Port, KEY16_Pin);

}
