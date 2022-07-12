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
#include<stdbool.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char oldKeyPressed = '\0';
char keyPressed = '\0';
char password[6] = "122455";
const int nbDigitsPassword = 6;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
char digicode_read();
char get_digicode_key();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
char digicode_read()
{
	HAL_GPIO_WritePin(col_2_GPIO_Port, col_2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(col_3_GPIO_Port, col_3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(col_4_GPIO_Port, col_4_Pin, GPIO_PIN_RESET);

	//COLONNE 1
	HAL_GPIO_WritePin(col_1_GPIO_Port, col_1_Pin, GPIO_PIN_SET);
	GPIO_PinState line1 = HAL_GPIO_ReadPin(lin_1_GPIO_Port, lin_1_Pin);
	GPIO_PinState line2 = HAL_GPIO_ReadPin(lin_2_GPIO_Port, lin_2_Pin);
	GPIO_PinState line3 = HAL_GPIO_ReadPin(lin_3_GPIO_Port, lin_3_Pin);
	GPIO_PinState line4 = HAL_GPIO_ReadPin(lin_4_GPIO_Port, lin_4_Pin);
	if(line1)
		return '1';
	if(line2)
		return '4';
	if(line3)
		return '7';
	if(line4)
		return '*';
	HAL_GPIO_WritePin(col_1_GPIO_Port, col_1_Pin, GPIO_PIN_RESET);

	//COLONNE 2
	HAL_GPIO_WritePin(col_2_GPIO_Port, col_2_Pin, GPIO_PIN_SET);
	line1 = HAL_GPIO_ReadPin(lin_1_GPIO_Port, lin_1_Pin);
	line2 = HAL_GPIO_ReadPin(lin_2_GPIO_Port, lin_2_Pin);
	line3 = HAL_GPIO_ReadPin(lin_3_GPIO_Port, lin_3_Pin);
	line4 = HAL_GPIO_ReadPin(lin_4_GPIO_Port, lin_4_Pin);
	if(line1)
		return '2';
	if(line2)
		return '5';
	if(line3)
		return '8';
	if(line4)
		return '0';
	HAL_GPIO_WritePin(col_2_GPIO_Port, col_2_Pin, GPIO_PIN_RESET);

	//COLONNE 3
	HAL_GPIO_WritePin(col_3_GPIO_Port, col_3_Pin, GPIO_PIN_SET);
	line1 = HAL_GPIO_ReadPin(lin_1_GPIO_Port, lin_1_Pin);
	line2 = HAL_GPIO_ReadPin(lin_2_GPIO_Port, lin_2_Pin);
	line3 = HAL_GPIO_ReadPin(lin_3_GPIO_Port, lin_3_Pin);
	line4 = HAL_GPIO_ReadPin(lin_4_GPIO_Port, lin_4_Pin);
	if(line1)
		return '3';
	if(line2)
		return '6';
	if(line3)
		return '9';
	if(line4)
		return '#';
	HAL_GPIO_WritePin(col_3_GPIO_Port, col_3_Pin, GPIO_PIN_RESET);

	//COLONNE 4
	HAL_GPIO_WritePin(col_4_GPIO_Port, col_4_Pin, GPIO_PIN_SET);
	line1 = HAL_GPIO_ReadPin(lin_1_GPIO_Port, lin_1_Pin);
	line2 = HAL_GPIO_ReadPin(lin_2_GPIO_Port, lin_2_Pin);
	line3 = HAL_GPIO_ReadPin(lin_3_GPIO_Port, lin_3_Pin);
	line4 = HAL_GPIO_ReadPin(lin_4_GPIO_Port, lin_4_Pin);
	if(line1)
		return 'A';
	if(line2)
		return 'B';
	if(line3)
		return 'C';
	if(line4)
		return 'D';
	HAL_GPIO_WritePin(col_4_GPIO_Port, col_4_Pin, GPIO_PIN_RESET);

	return '\0';
}

char get_digicode_key()
{
	keyPressed = digicode_read();
	if(keyPressed == '\0' && oldKeyPressed != '\0')
	{
	 HAL_Delay(200);
	}
	if(keyPressed != '\0' && oldKeyPressed == '\0')
	{
		//Touche entrée
		oldKeyPressed = keyPressed;
		return keyPressed;
	}
	oldKeyPressed = keyPressed;
	return NULL;

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int passwordIndex = 0;
  bool isOpen = false;
  bool isChangingPassword = false;
  char closeChar = 'A';
  char changePasswordChar = 'D';
  char currentPassword[6] = "ZZZZZZ";
  HAL_Delay(500);
  while (1)
  {
	  char c = get_digicode_key();
	  if(c != NULL)
		  HAL_UART_Transmit(&huart2, &c, sizeof(c), 1000);
	  
	  //begin Léonard's code
	  if(!isOpen)
	  {
		  if(c != NULL)
		  {
			  currentPassword[passwordIndex] = c;
			  passwordIndex++;
			  if(passwordIndex >= nbDigitsPassword)
			  {
				  //on vérif que le mdr est le bon
				  bool isGoodPassword = true;
				  for(int i = 0; i < nbDigitsPassword; i++)
				  {
					  if(password[i] != currentPassword[i])
					  {
						  isGoodPassword = false;
						  break;
					  }
				  }
				  if(isGoodPassword)
				  {
					  //ON ouvre la porte
					  HAL_UART_Transmit(&huart2, "Code bon\n", sizeof("Code bon\n"), 1000);
					  isOpen = true;
				  }
				  else
				  {
					  //Code erroré, allumé la led rouge
					  HAL_UART_Transmit(&huart2, "Code errone\n", sizeof("Code errone\n"), 1000);
				  }
				  passwordIndex = 0;
			  }
		  }
	  }
	  else//Porte ouverte
	  {
		  if(c != NULL)
		  {
			  if(c == changePasswordChar && !isChangingPassword)
			  {
				  isChangingPassword = true;
			  }
			  else if(c == closeChar)
			  {
				  HAL_UART_Transmit(&huart2, "Fermeture de porte\n", sizeof("Fermeture de porte\n"), 1000);
				  isOpen = false;
				  isChangingPassword = false;
			  }
			  else if(isChangingPassword)
			  {
				  password[passwordIndex] = c;
				  passwordIndex++;
				  if(passwordIndex >= nbDigitsPassword)
				  {
					  HAL_UART_Transmit(&huart2, "mdp change\n", sizeof("mdp change\n"), 1000);
					  isChangingPassword = false;
					  passwordIndex = 0;
				  }
			  }
		  }	  
	  }

	  
	  //end Léonard's code
	  
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
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
  huart2.Init.BaudRate = 115200;
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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|col_3_Pin|col_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(col_4_GPIO_Port, col_4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(col_1_GPIO_Port, col_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin col_3_Pin col_2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|col_3_Pin|col_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : col_4_Pin */
  GPIO_InitStruct.Pin = col_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(col_4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : col_1_Pin */
  GPIO_InitStruct.Pin = col_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(col_1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : lin_4_Pin */
  GPIO_InitStruct.Pin = lin_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(lin_4_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : lin_3_Pin lin_1_Pin lin_2_Pin */
  GPIO_InitStruct.Pin = lin_3_Pin|lin_1_Pin|lin_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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