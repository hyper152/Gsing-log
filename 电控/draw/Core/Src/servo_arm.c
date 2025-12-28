#include "servo_arm.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>  // 用于abs()
#define M_PI 3.14159265358979323846f  // 避免M_PI未定义

// 机械臂硬件参数（保持不变）
#define BIG_ARM_LEN    100.0f
#define SMALL_ARM_LEN  100.0f
#define PAPER_DISTANCE 120.0f

// 舵机角度限制（保持不变）
#define PAN_MIN_ANGLE  60.0f     // 云台：60~120°
#define PAN_MAX_ANGLE  120.0f
#define BIG_ARM_MIN    60.0f     // 大臂：60~120°
#define BIG_ARM_MAX    120.0f
#define SMALL_ARM_MIN  70.0f     // 小臂：70~110°
#define SMALL_ARM_MAX  110.0f

// 舵机通道定义（保持与.h一致，关键修改）
#define PAN_SERVO_CHANNEL    TIM_CHANNEL_1  // TIM2_CH1=PA0
#define BIG_ARM_SERVO_CHANNEL TIM_CHANNEL_1  // TIM1_CH1=PA8
#define SMALL_ARM_SERVO_CHANNEL TIM_CHANNEL_3// TIM2_CH3=PA2

// 声明外部定时器变量（TIM1和TIM2）
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

// 舵机初始化（关键修改：启动TIM1_CH1的PWM，大臂用TIM1）
void Servo_Init(void)
{
    // 启动原有定时器通道
    HAL_TIM_PWM_Start(&htim2, PAN_SERVO_CHANNEL);    // 云台-PA0（TIM2_CH1）
    HAL_TIM_PWM_Start(&htim2, SMALL_ARM_SERVO_CHANNEL); // 小臂-PA2（TIM2_CH3）
    HAL_TIM_PWM_Start(&htim1, BIG_ARM_SERVO_CHANNEL); // 大臂-PA8（TIM1_CH1）→ 新增TIM1启动

    // 强制所有舵机回到中间位置（90°）
    uint32_t mid_pulse = 500 + (90.0f / 180.0f) * 2000;
    __HAL_TIM_SET_COMPARE(&htim2, PAN_SERVO_CHANNEL, mid_pulse);    // 云台
    __HAL_TIM_SET_COMPARE(&htim1, BIG_ARM_SERVO_CHANNEL, mid_pulse); // 大臂（用TIM1）
    __HAL_TIM_SET_COMPARE(&htim2, SMALL_ARM_SERVO_CHANNEL, mid_pulse); // 小臂

    Delay_ms(3000); // 延长延时，确保舵机完全稳定
}

// 舵机角度控制（关键修改：大臂通道关联TIM1）
void Servo_SetAngle(TIM_HandleTypeDef *htim, uint32_t channel, float angle, float big_arm_angle)
{
    // 1. 严格角度限幅（保持不变）
    if (channel == PAN_SERVO_CHANNEL)
        angle = angle < PAN_MIN_ANGLE ? PAN_MIN_ANGLE : (angle > PAN_MAX_ANGLE ? PAN_MAX_ANGLE : angle);
    else if (channel == BIG_ARM_SERVO_CHANNEL)
        angle = angle < BIG_ARM_MIN ? BIG_ARM_MIN : (angle > BIG_ARM_MAX ? BIG_ARM_MAX : angle);
    else if (channel == SMALL_ARM_SERVO_CHANNEL)
        angle = angle < SMALL_ARM_MIN ? SMALL_ARM_MIN : (angle > SMALL_ARM_MAX ? SMALL_ARM_MAX : angle);

    // 2. 角度转PWM（保持不变）
    uint32_t pulse = (uint32_t)(500 + (angle / 180.0f) * 2000);
    pulse = pulse < 500 ? 500 : (pulse > 2500 ? 2500 : pulse);

    // 3. 脉冲变化≥5us才更新（保持不变）
    uint32_t current_pulse = __HAL_TIM_GET_COMPARE(htim, channel);
    if (abs((int)pulse - (int)current_pulse) >= 5)
    {
        __HAL_TIM_SET_COMPARE(htim, channel, pulse);
    }
}

static void InverseKinematics(float x, float y, float *pan_angle, float *big_arm_angle, float *small_arm_angle)
{
    /************************** 1. 云台角度计算 **************************/
    *pan_angle = 90.0f + (x / 60.0f) * 30.0f;  // x±60mm → 角度±30°
    *pan_angle = (*pan_angle < PAN_MIN_ANGLE) ? PAN_MIN_ANGLE :
                 (*pan_angle > PAN_MAX_ANGLE) ? PAN_MAX_ANGLE : *pan_angle;

    /************************** 2. 修正前后运动计算（关键修改） **************************/
    // y坐标直接控制前后距离，正值向前，负值向后
    float effective_distance = PAPER_DISTANCE + y;  // y影响实际工作距离

    // 限制有效距离范围
    float min_distance = 80.0f, max_distance = 160.0f;
    effective_distance = (effective_distance < min_distance) ? min_distance :
                        (effective_distance > max_distance) ? max_distance : effective_distance;

    /************************** 3. 简化的逆运动学计算 **************************/
    float L1 = BIG_ARM_LEN, L2 = SMALL_ARM_LEN;
    float D = effective_distance;  // 使用修正后的距离

    // 计算小臂关节角度（余弦定理）
    float cos_angle2 = (L1*L1 + L2*L2 - D*D) / (2.0f * L1 * L2);
    cos_angle2 = (cos_angle2 < -1.0f) ? -1.0f : (cos_angle2 > 1.0f) ? 1.0f : cos_angle2;
    float angle2 = acosf(cos_angle2) * 180.0f / M_PI;

    // 计算大臂角度
    float sin_angle2 = sinf(angle2 * M_PI / 180.0f);
    float k1 = L1 + L2 * cosf(angle2 * M_PI / 180.0f);
    float k2 = L2 * sin_angle2;
    float angle1 = atan2f(k2, k1) * 180.0f / M_PI;

    /************************** 4. 角度映射到舵机 **************************/
    *big_arm_angle = 90.0f + angle1;
    *small_arm_angle = 180.0f - angle2;

    /************************** 5. 最终角度限制 **************************/
    *big_arm_angle = (*big_arm_angle < BIG_ARM_MIN) ? BIG_ARM_MIN :
                     (*big_arm_angle > BIG_ARM_MAX) ? BIG_ARM_MAX : *big_arm_angle;
    *small_arm_angle = (*small_arm_angle < SMALL_ARM_MIN) ? SMALL_ARM_MIN :
                       (*small_arm_angle > SMALL_ARM_MAX) ? SMALL_ARM_MAX : *small_arm_angle;

    // 调试输出（建议启用查看计算值）
    // printf("x=%.1f, y=%.1f, eff_dist=%.1f -> pan=%.1f, big=%.1f, small=%.1f\n",
    //        x, y, effective_distance, *pan_angle, *big_arm_angle, *small_arm_angle);
}

// 机械臂移动（关键修改：大臂调用TIM1）
void Arm_MoveToPoint(float x, float y)
{
    float pan_angle, big_arm_angle, small_arm_angle;
    InverseKinematics(x, y, &pan_angle, &big_arm_angle, &small_arm_angle);

    Servo_SetAngle(&htim2, PAN_SERVO_CHANNEL, pan_angle, 0.0f);    // 云台-TIM2
    Delay_ms(600);
    Servo_SetAngle(&htim1, BIG_ARM_SERVO_CHANNEL, big_arm_angle, 0.0f); // 大臂-TIM1 → 关键修改
    Delay_ms(1000);
    Servo_SetAngle(&htim2, SMALL_ARM_SERVO_CHANNEL, small_arm_angle, big_arm_angle); // 小臂-TIM2
    Delay_ms(1000);
}

void Arm_DrawSquare(void)
{
    // 扩大正方形尺寸 - 使用更大的坐标范围
    float square_points[5][2] = {
        {-30.0f, -30.0f},  // 左下
        {30.0f, -30.0f},   // 右下
        {30.0f, 30.0f},    // 右上
        {-30.0f, 30.0f},   // 左上
        {-30.0f, -30.0f}   // 回到左下（闭合）
    };

    // 抬起笔的位置（更高的位置）
    Arm_MoveToPoint(0.0f, -40.0f);
    Delay_ms(2000);

    // 绘制正方形
    for (int i = 0; i < 5; i++) {
        Arm_MoveToPoint(square_points[i][0], square_points[i][1]);
        Delay_ms(1500);
    }

    // 回到初始位置
    Arm_MoveToPoint(0.0f, -40.0f);
    Delay_ms(1000);
}

// 延时函数（保持不变）
void Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
