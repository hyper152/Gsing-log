/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ax_ps2.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PWM_MAX 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
JOYSTICK_TypeDef ps2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void PWM_Init(void);
void PWM_SetDuty(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t duty);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void PWM_Init(void) {
	// 初始化TIM1（PA8-CH1、PA9-CH2、PA10-CH3、PA11-CH4）
	    htim1.Instance = TIM1;
	    htim1.Init.Prescaler = 71;
	    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	    htim1.Init.Period = PWM_MAX - 1;
	    htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	    htim1.Init.RepetitionCounter = 0;
	    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	    if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
	        Error_Handler();
	    }

	    TIM_OC_InitTypeDef sConfigOC = {0};
	    sConfigOC.OCMode = TIM_OCMODE_PWM1;
	    sConfigOC.Pulse = 0;
	    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
	    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
	    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) Error_Handler();
	    if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) Error_Handler();

	    // 初始化TIM4（PB6-CH1、PB7-CH2、PB8-CH3）
	    htim4.Instance = TIM4;
	    htim4.Init.Prescaler = 71;
	    htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	    htim4.Init.Period = PWM_MAX - 1;
	    htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	    htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	    if (HAL_TIM_PWM_Init(&htim4) != HAL_OK) {
	        Error_Handler();
	    }

	    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) Error_Handler();
	    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) Error_Handler();
	    if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) Error_Handler();

	    // 初始化TIM3（PB5-CH2）
	    htim3.Instance = TIM3;
	    htim3.Init.Prescaler = 71;
	    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	    htim3.Init.Period = PWM_MAX - 1;
	    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
	        Error_Handler();
	    }

	    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) Error_Handler();

    // 启动PWM输出
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

    __HAL_TIM_MOE_ENABLE(&htim1);
}

void PWM_SetDuty(TIM_HandleTypeDef *htim, uint32_t channel, uint16_t duty) {
    if (duty > PWM_MAX) duty = PWM_MAX;
    __HAL_TIM_SET_COMPARE(htim, channel, duty);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  AX_PS2_Init();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  PWM_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    AX_PS2_ScanKey(&ps2);

    // 按键定义：
    // ps2.btn1 == 64  → 后退（保持原逻辑）
    // ps2.btn1 == 16  → 前进
    // ps2.btn1 == 128 → 左转
    // ps2.btn1 == 32  → 右转

    if (ps2.btn2 == 16) {
        // 后退（保留原可行逻辑）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 1000); // PA9（右前反转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 1000); // PA11（左前反转）
        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 1000); // PB6（右后反转）
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 1000); // PB7（左后反转）
        // 关闭其他通道
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 0);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 0);
        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 0);
    }
    else if (ps2.btn2 == 64) {
        // 前进：所有电机正向转动（占空比与后退一致，确保速度匹配）
         PWM_SetDuty(&htim1, TIM_CHANNEL_2, 0); // PA9（右前反转）
         PWM_SetDuty(&htim1, TIM_CHANNEL_4, 0); // PA11（左前反转）
         PWM_SetDuty(&htim4, TIM_CHANNEL_1, 0); // PB6（右后反转）
         PWM_SetDuty(&htim4, TIM_CHANNEL_2, 0); // PB7（左后反转）
         // 关闭其他通道
         PWM_SetDuty(&htim1, TIM_CHANNEL_1, 1000);
         PWM_SetDuty(&htim1, TIM_CHANNEL_3, 1000);
         PWM_SetDuty(&htim3, TIM_CHANNEL_2, 1000);
         PWM_SetDuty(&htim4, TIM_CHANNEL_3, 1000);
    }
    else if (ps2.btn2 == 128) {
        // 右转：右侧电机停转/低速，左侧电机正转（利用前进方向的左侧电机）
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 0); // PA8（右前正转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 1000);

        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 1000);   // 左前停转
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 0);

        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 1000);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 0);

        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 1000);   // 左后停转
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 0); // PB7（右后正转）
    }
    else if (ps2.btn2 == 32) {
        // 右转：右侧电机停转/低速，左侧电机正转（利用前进方向的左侧电机）
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 800); // PA8（右前正转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 0);

        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 0);   // 左前停转
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 800);

        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 800);

        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 0);   // 左后停转
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 800); // PB7（右后正转）
    }
    else if (ps2.btn1 == 16) {
        // 后退（保留原可行逻辑）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 600); // PA9（右前反转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 600); // PA11（左前反转）
        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 600); // PB6（右后反转）
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 600); // PB7（左后反转）
        // 关闭其他通道
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 0);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 0);
        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 0);
    }
    else if (ps2.btn1 == 64) {
        // 前进：所有电机正向转动（占空比与后退一致，确保速度匹配）
         PWM_SetDuty(&htim1, TIM_CHANNEL_2, 0); // PA9（右前反转）
         PWM_SetDuty(&htim1, TIM_CHANNEL_4, 0); // PA11（左前反转）
         PWM_SetDuty(&htim4, TIM_CHANNEL_1, 0); // PB6（右后反转）
         PWM_SetDuty(&htim4, TIM_CHANNEL_2, 0); // PB7（左后反转）
         // 关闭其他通道
         PWM_SetDuty(&htim1, TIM_CHANNEL_1, 600);
         PWM_SetDuty(&htim1, TIM_CHANNEL_3, 600);
         PWM_SetDuty(&htim3, TIM_CHANNEL_2, 600);
         PWM_SetDuty(&htim4, TIM_CHANNEL_3, 600);
    }
    else if (ps2.btn1 == 128) {
        // 右转：右侧电机停转/低速，左侧电机正转（利用前进方向的左侧电机）
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 0); // PA8（右前正转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 600);

        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 600);   // 左前停转
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 0);

        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 600);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 0);

        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 600);   // 左后停转
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 0); // PB7（右后正转）
    }
    else if (ps2.btn1 == 32) {
        // 右转：右侧电机停转/低速，左侧电机正转（利用前进方向的左侧电机）
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 800); // PA8（右前正转）
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 0);

        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 0);   // 左前停转
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 800);

        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 800);

        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 0);   // 左后停转
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 800); // PB7（右后正转）
    }


    else {
        // 无按键按下时，所有电机停转
        PWM_SetDuty(&htim1, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim1, TIM_CHANNEL_2, 0);
        PWM_SetDuty(&htim1, TIM_CHANNEL_3, 0);
        PWM_SetDuty(&htim1, TIM_CHANNEL_4, 0);
        PWM_SetDuty(&htim4, TIM_CHANNEL_1, 0);
        PWM_SetDuty(&htim4, TIM_CHANNEL_2, 0);
        PWM_SetDuty(&htim4, TIM_CHANNEL_3, 0);
        PWM_SetDuty(&htim3, TIM_CHANNEL_2, 0);


    }

    HAL_Delay(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
