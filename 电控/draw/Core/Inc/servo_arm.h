#ifndef INC_SERVO_ARM_H_
#define INC_SERVO_ARM_H_

#ifndef __SERVO_ARM_H
#define __SERVO_ARM_H

#include "stm32f1xx_hal.h"
#include "tim.h"

// 机械臂硬件参数（保持不变）
#define BIG_ARM_LEN    100.0f    // 大臂长度(mm)
#define SMALL_ARM_LEN  100.0f    // 小臂长度(mm)
#define END_LEN        60.0f     // 末端机构长度(mm)
#define PAPER_DISTANCE 120.0f    // 云台中心到纸面距离(mm)

// 舵机通道定义（关键修改：大臂改为TIM1_CH1=PA8）
#define PAN_SERVO_CHANNEL    TIM_CHANNEL_1  // 云台舵机-PA0（TIM2_CH1）
#define BIG_ARM_SERVO_CHANNEL TIM_CHANNEL_1  // 大臂舵机-PA8（TIM1_CH1）→ 通道改为1，定时器改为TIM1
#define SMALL_ARM_SERVO_CHANNEL TIM_CHANNEL_3// 小臂舵机-PA2（TIM2_CH3）

// 新增：声明TIM1为外部变量（大臂用TIM1）
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

// 函数声明（保持不变）
void Servo_Init(void);                          // 舵机初始化（启动PWM输出）
void Servo_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, float angle, float big_arm_angle);
void Arm_MoveToPoint(float x, float y);         // 机械臂移动到2D坐标点(x,y)
void Arm_DrawSquare(void);                      // 绘制正方形（示例图案）
void Arm_DrawTriangle(void);                    // 绘制三角形（扩展图案）
void Delay_ms(uint32_t ms);                     // 毫秒延时函数

#endif

#endif /* INC_SERVO_ARM_H_ */
