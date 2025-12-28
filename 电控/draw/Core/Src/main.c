#include "main.h"
#include "tim.h"
#include "stm32f1xx_hal.h"
#include "servo_arm.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM1_Init(); // 关键新增：初始化TIM1（大臂用）
    MX_TIM2_Init(); // 保持TIM2初始化（云台、小臂用）

    Servo_Init(); // 初始化+居中校准（3秒延时）

    Arm_DrawSquare();
    Delay_ms(2000);

    Servo_Init(); // 初始化+居中校准（3秒延时）
    while (1)
    {

    }

}

// 系统时钟配置（保持不变，注意：若用HSI+PLL_MUL16，实际时钟是64MHz，不影响PWM频率）
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2; // 4MHz
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16; // 4*16=64MHz（合法倍频）
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2; // APB1=32MHz
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void)
{
    // 无需额外配置，TIM1/TIM2引脚在tim.c中配置
}

void Error_Handler(void)
{
    while (1)
    {
        // 错误提示（可选：接LED闪烁）
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
