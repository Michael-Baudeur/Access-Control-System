/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
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
CAN_HandleTypeDef hcan1;

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char secret_code[4] = "1234";
char test_secret_code[15];
int i = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
int _write(int file, char *ptr, int len)
{
	int i=0;
	for(i=0 ; i<len ; i++)
	{
		ITM_SendChar((*ptr++));
	}
	return len;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char digicode_read(void) {

	//column 1
	HAL_GPIO_WritePin(col_1_output_GPIO_Port, col_1_output_Pin, GPIO_PIN_SET);
	int key_1 = HAL_GPIO_ReadPin(row_5_input_GPIO_Port, row_5_input_Pin);
	if(key_1==1) {
		return '1';
	}
	int key_4 = HAL_GPIO_ReadPin(row_6_input_GPIO_Port, row_6_input_Pin);
	if(key_4==1) {
		return '4';
	}
	int key_7 = HAL_GPIO_ReadPin(row_7_input_GPIO_Port, row_7_input_Pin);
	if(key_7==1) {
		return '7';
	}
	int key_star = HAL_GPIO_ReadPin(row_8_input_GPIO_Port, row_8_input_Pin);
	if(key_star==1) {
		return '*';
	}
	HAL_GPIO_WritePin(col_1_output_GPIO_Port, col_1_output_Pin, GPIO_PIN_RESET);

	//column 2
	HAL_GPIO_WritePin(col_2_output_GPIO_Port, col_2_output_Pin, GPIO_PIN_SET);
	int key_2 = HAL_GPIO_ReadPin(row_5_input_GPIO_Port, row_5_input_Pin);
	if(key_2==1) {
		return '2';
	}
	int key_5 = HAL_GPIO_ReadPin(row_6_input_GPIO_Port, row_6_input_Pin);
	if(key_5==1) {
		return '5';
	}
	int key_8 = HAL_GPIO_ReadPin(row_7_input_GPIO_Port, row_7_input_Pin);
	if(key_8==1) {
		return '8';
	}
	int key_0 = HAL_GPIO_ReadPin(row_8_input_GPIO_Port, row_8_input_Pin);
	if(key_0==1) {
		return '0';
	}
	HAL_GPIO_WritePin(col_2_output_GPIO_Port, col_2_output_Pin, GPIO_PIN_RESET);

	//column 3
	HAL_GPIO_WritePin(col_3_output_GPIO_Port, col_3_output_Pin, GPIO_PIN_SET);
	int key_3 = HAL_GPIO_ReadPin(row_5_input_GPIO_Port, row_5_input_Pin);
	if(key_3==1) {
		return '3';
	}
	int key_6 = HAL_GPIO_ReadPin(row_6_input_GPIO_Port, row_6_input_Pin);
	if(key_6==1) {
		return '6';
	}
	int key_9 = HAL_GPIO_ReadPin(row_7_input_GPIO_Port, row_7_input_Pin);
	if(key_9==1) {
		return '9';
	}
	int key_sharp = HAL_GPIO_ReadPin(row_8_input_GPIO_Port, row_8_input_Pin);
	if(key_sharp==1) {
		return '#';
	}
	HAL_GPIO_WritePin(col_3_output_GPIO_Port, col_3_output_Pin, GPIO_PIN_RESET);

	//column 4
	HAL_GPIO_WritePin(col_4_output_GPIO_Port, col_4_output_Pin, GPIO_PIN_SET);
	int key_A = HAL_GPIO_ReadPin(row_5_input_GPIO_Port, row_5_input_Pin);
	if(key_A) {
		return 'A';
	}
	int key_B = HAL_GPIO_ReadPin(row_6_input_GPIO_Port, row_6_input_Pin);
	if(key_B) {
		return 'B';
	}
	int key_C = HAL_GPIO_ReadPin(row_7_input_GPIO_Port, row_7_input_Pin);
	if(key_C) {
		return 'C';
	}
	int key_D = HAL_GPIO_ReadPin(row_8_input_GPIO_Port, row_8_input_Pin);
	if(key_D) {
		return 'D';
	}
	HAL_GPIO_WritePin(col_4_output_GPIO_Port, col_4_output_Pin, GPIO_PIN_RESET);

	return '\0';
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_UART_Transmit(&huart2, "test_init\n", sizeof("test_init\n"), 10000);// print

  // Variables a stocker
  char oldButtonPressed = '\0';


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while (1)
  {
	  char newButtonPressed = digicode_read();

	  if(oldButtonPressed == '\0' && newButtonPressed != '\0')
	  {
		  //on vient d'appuyer sur le boutton newButtonPressed

	  }
	  oldButtonPressed = newButtonPressed;
	  //HAL_UART_Transmit(&huart2, &newButtonPressed, sizeof(newButtonPressed), 10000);
	  HAL_UART_Transmit(&huart2, "salut", sizeof("salut"), 10000);
	  /*
	  int key_1 = HAL_GPIO_ReadPin(row_5_input_GPIO_Port, row_5_input_Pin);
	  if(key_1==0) {
		  HAL_GPIO_TogglePin(GPIOx, GPIO_Pin)
	  }
	  else {
		  HAL_GPIO_WritePin(GPIOA, red_led_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(GPIOA, green_led_Pin, GPIO_PIN_SET);

	  }*/

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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5|red_led_Pin|green_led_Pin|col_3_output_Pin
                          |col_2_output_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, col_4_output_Pin|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(col_1_output_GPIO_Port, col_1_output_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA5 red_led_Pin green_led_Pin col_3_output_Pin
                           col_2_output_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_5|red_led_Pin|green_led_Pin|col_3_output_Pin
                          |col_2_output_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : col_4_output_Pin PB6 */
  GPIO_InitStruct.Pin = col_4_output_Pin|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : col_1_output_Pin */
  GPIO_InitStruct.Pin = col_1_output_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(col_1_output_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : lin_4_input_Pin */
  GPIO_InitStruct.Pin = lin_4_input_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(lin_4_input_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : lin_3_input_Pin lin_1_input_Pin lin_2_input_Pin */
  GPIO_InitStruct.Pin = lin_3_input_Pin|lin_1_input_Pin|lin_2_input_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	//to_input();


	HAL_UART_Transmit(&huart2, "test\n", sizeof("test\n"), 10000);
	char* code = digicode_read();
	HAL_UART_Transmit(&huart2, "test2\n", sizeof("test2\n"), 10000);
	HAL_UART_Transmit(&huart2, code, sizeof(code), 10000);

	//to_irq();
}*/
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
