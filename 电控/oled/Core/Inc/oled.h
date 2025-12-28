#ifndef OLED_H
#define OLED_H

#include "stm32f1xx_hal.h"

// OLED I2C地址（根据硬件调整，0x3C或0x3D）
#define OLED_ADDR 0x3C << 1

// 屏幕尺寸
#define OLED_WIDTH        128
#define OLED_HEIGHT       64

// 函数声明
void OLED_Init(I2C_HandleTypeDef *hi2c);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x, uint8_t y, const uint8_t *char_data);
void OLED_ShowMainLogic(void);


#endif
