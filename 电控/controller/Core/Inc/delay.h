#ifndef __DELAY_H
#define __DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"  // 依赖 HAL 库

/**
 * @brief 微秒级延时（基于 SysTick）
 * @param us: 延时的微秒数（范围 1~1000）
 * @note 适用于 STM32 HAL 库，依赖 SystemCoreClock 变量
 */
void delay_us(uint16_t us);

#ifdef __cplusplus
}
#endif

#endif /* __DELAY_H */
