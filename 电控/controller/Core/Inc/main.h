/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H  // 防止头文件重复包含的宏定义
#define __MAIN_H

#ifdef __cplusplus  // 兼容C++编译环境
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"  // 引入STM32F1系列HAL库核心头文件，包含外设驱动、寄存器定义等

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */  // 用户自定义头文件包含区域（可添加项目所需头文件，如oled.h、nrf24l01.h等）
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */  // 用户自定义数据类型声明区域（如结构体、枚举等）
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */  // 用户自定义全局常量声明区域（如#define宏、const常量等）
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */  // 用户自定义宏定义区域（如功能宏、条件编译宏等）
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);  // 错误处理函数原型声明，程序异常时调用

/* USER CODE BEGIN EFP */  // 用户自定义函数原型声明区域（如外设初始化、数据处理函数等）
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
// 摇杆引脚定义（模拟输入，连接ADC通道）
#define LX_Pin GPIO_PIN_0        // 左摇杆X轴 → PA0（ADC通道0）
#define LX_GPIO_Port GPIOA
#define LY_Pin GPIO_PIN_1        // 左摇杆Y轴 → PA1（ADC通道1）
#define LY_GPIO_Port GPIOA
#define RX_Pin GPIO_PIN_2        // 右摇杆X轴 → PA2（ADC通道2）
#define RX_GPIO_Port GPIOA
#define RY_Pin GPIO_PIN_3        // 右摇杆Y轴 → PA3（ADC通道3）
#define RY_GPIO_Port GPIOA

// ===================== 按键按编号顺序标注 =====================
#define KEY1_Pin GPIO_PIN_4      // 按键1 → PA4
#define KEY1_GPIO_Port GPIOA
#define KEY2_Pin GPIO_PIN_8      // 按键2 → PA8
#define KEY2_GPIO_Port GPIOA
// 注：原文件未定义KEY3引脚，需确认硬件连接
#define KEY4_Pin GPIO_PIN_11     // 按键4 → PA11
#define KEY4_GPIO_Port GPIOA
#define KEY5_Pin GPIO_PIN_12     // 按键5 → PA12
#define KEY5_GPIO_Port GPIOA
#define KEY6_Pin GPIO_PIN_15     // 按键6 → PA15
#define KEY6_GPIO_Port GPIOA
#define KEY7_Pin GPIO_PIN_2      // 按键7 → PB2
#define KEY7_GPIO_Port GPIOB
#define KEY8_Pin GPIO_PIN_3      // 按键8 → PB3
#define KEY8_GPIO_Port GPIOB
#define KEY9_Pin GPIO_PIN_4      // 按键9 → PB4
#define KEY9_GPIO_Port GPIOB
#define KEY10_Pin GPIO_PIN_5     // 按键10 → PB5
#define KEY10_GPIO_Port GPIOB
#define KEY11_Pin GPIO_PIN_8     // 按键11 → PB8
#define KEY11_GPIO_Port GPIOB
#define KEY12_Pin GPIO_PIN_9     // 按键12 → PB9
#define KEY12_GPIO_Port GPIOB
#define KEY13_Pin GPIO_PIN_10    // 按键13 → PB10
#define KEY13_GPIO_Port GPIOB
#define KEY14_Pin GPIO_PIN_11    // 按键14 → PB11
#define KEY14_GPIO_Port GPIOB
#define KEY15_Pin GPIO_PIN_12    // 按键15 → PB12
#define KEY15_GPIO_Port GPIOB
#define KEY16_Pin GPIO_PIN_5     // 按键16 → PA5
#define KEY16_GPIO_Port GPIOA

// 旋钮/额外模拟输入引脚
#define RRO_Pin GPIO_PIN_6       // 右侧旋钮/模拟输入 → PA6（ADC通道6）
#define RRO_GPIO_Port GPIOA
#define LRO_Pin GPIO_PIN_7       // 左侧旋钮/模拟输入 → PA7（ADC通道7）
#define LRO_GPIO_Port GPIOA

// NRF24L01无线模块控制引脚（数字输出）
#define CE_Pin GPIO_PIN_0        // NRF24L01 CE引脚 → PB0
#define CE_GPIO_Port GPIOB
#define CSN_Pin GPIO_PIN_1       // NRF24L01 CSN引脚 → PB1
#define CSN_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */  // 用户自定义私有宏定义区域（如硬件参数、功能开关等）
/* USER CODE END Private defines */

#ifdef __cplusplus  // 兼容C++编译环境
}
#endif

#endif /* __MAIN_H */  // 头文件结束标记
