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
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "NRF24L01.h"
#include "oled.h"
#include <stdio.h>
#include "button.h"
#include "axis.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t KeyNum;

uint8_t SendFlag;								//发送标志位
uint8_t SendSuccessCount, SendFailedCount;		//发送成功计次，发送失败计次

uint8_t ReceiveFlag;							//接收标志位
uint8_t ReceiveSuccessCount, ReceiveFailedCount;//接收成功计次，接收失败计次
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();



  /* USER CODE BEGIN 2 */
  HAL_Delay(50);
//  OLED_Init();
//  NRF24L01_Init();
  Axis_init();
  SendSuccessCount = 0;
  SendFailedCount = 0;
  ReceiveSuccessCount = 0;
  ReceiveFailedCount = 0;



//  HAL_Delay(1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
         Button_read();
         Axis_read();


         memset(NRF24L01_TxPacket, 0, 8);

             //  填充有效数据（前8字节）
             NRF24L01_TxPacket[0] = L[0];      // 左摇杆X
             NRF24L01_TxPacket[1] = L[1];      // 左摇杆Y
             NRF24L01_TxPacket[2] = R[0];      // 右摇杆X
             NRF24L01_TxPacket[3] = R[1];      // 右摇杆Y
             NRF24L01_TxPacket[4] = Round[0];  // 旋钮1
             NRF24L01_TxPacket[5] = Round[1];  // 旋钮2

             //  打包按键状态
             for(uint8_t i=0; i<8; i++) {
                 if(button[i])   NRF24L01_TxPacket[6] |= (1 << i);
                 if(button[i+8]) NRF24L01_TxPacket[7] |= (1 << i);
             }
             //  发送并接收
             SendFlag = NRF24L01_Send();

			if (SendFlag == 1)			//发送标志位为1，表示发送成功
			{
				SendSuccessCount ++;	//发送成功计次变量自增
			}
			else						//发送标志位不为1，即2/3/4，表示发送不成功
			{
				SendFailedCount ++;		//发送失败计次变量自增
			}




			ReceiveFlag = NRF24L01_Receive();

			if (ReceiveFlag)				//接收标志位不为0，表示收到了一个数据包
			{
				if (ReceiveFlag == 1)		//接收标志位为1，表示接收成功
				{
					ReceiveSuccessCount ++;	//接收成功计次变量自增
				}
				else	//接收标志位不为0也不为1，即2/3，表示此次接收产生了错误，错误接收的数据不应该使用
				{
					ReceiveFailedCount ++;	//接收失败计次变量自增
				}
			}


		    //OLED_NewFrame();

		    // 显示摇杆和旋钮值
		    //char buf[32];
		    //sprintf(buf, "LX:%3d LY:%3d", L[0], L[1]);
		    //OLED_PrintASCIIString(0, 0, buf, &afont8x6, OLED_COLOR_NORMAL);

		    //sprintf(buf, "RX:%3d RY:%3d", R[0], R[1]);
		    //OLED_PrintASCIIString(0, 8, buf, &afont8x6, OLED_COLOR_NORMAL);

		    //sprintf(buf, "R1:%3d R2:%3d", Round[0], Round[1]);
		    //OLED_PrintASCIIString(0, 16, buf, &afont8x6, OLED_COLOR_NORMAL);

//		     显示发送/接收状态
		    //sprintf(buf, "Tx:%3d/%3d", SendSuccessCount, SendFailedCount);
		  //  OLED_PrintASCIIString(0, 24, buf, &afont8x6, OLED_COLOR_NORMAL);

		//    sprintf(buf, "Rx:%3d/%3d", ReceiveSuccessCount, ReceiveFailedCount);
//		    OLED_PrintASCIIString(0, 32, buf, &afont8x6, OLED_COLOR_NORMAL);



//		     显示当前时间
//		     这里可以添加RTC功能获取时间

	//	  OLED_ShowFrame();



	  //    HAL_Delay(10); // 每秒操作一次


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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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

#ifdef  USE_FULL_ASSERT
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
