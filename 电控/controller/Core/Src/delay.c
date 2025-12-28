#include "delay.h"  // 确保包含 HAL 头文件

/**
 * @brief 微秒级延时（基于 SysTick）
 * @param us: 延时的微秒数（1~1000）
 */
void delay_us(uint16_t us) {
    uint32_t ticks = us * (SystemCoreClock / 1000000);  // 计算需要的时钟周期数
    uint32_t start = HAL_GetTick();  // 获取当前 SysTick 值
    while ((HAL_GetTick() - start) < ticks) {
        // 等待
    }
}
