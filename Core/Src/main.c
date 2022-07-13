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
#include <stdbool.h>
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

RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char oldKeyPressed = '\0';
char keyPressed = '\0';
char password[6] = "122455";
const int nbDigitsPassword = 6;

//bus CAN
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_RTC_Init(void);
static void MX_UART4_Init(void);
static void MX_CAN1_Init(void);
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

void WriteLog(char* logMessage)
{
    FILE* file = NULL;
    file = fopen("Log.txt", "w+");
    if(file != NULL)
    {

      fputs(logMessage, file);
    }
}

bool CAN_Transmit(uint32_t addr, uint32_t data_size, uint8_t * tab_data)
{
    if(data_size > 8 || sizeof(tab_data) / sizeof(tab_data[0]) <= data_size)
    	return false;

    TxHeader.StdId = addr;
    TxHeader.DLC = data_size;

    for(int i = 0; i < data_size; i++)
    {
	TxData[i] = tab_data[i];
    }

    HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox);

    return true;
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
  MX_RTC_Init();
  MX_UART4_Init();
  MX_CAN1_Init();
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
					//on vérif que le mdp est le bon
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
						char* mess = strcat("Password Error : ", currentPassword);
						WriteLog(mess);
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
					HAL_UART_Transmit(&huart2, "En attente du nouveau mdp\n", sizeof("En attente du nouveau mdp\n"), 1000);
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
	    // La transmision de mess avec le bus CAN
	    if(HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0) > 0)
	    {
	    	HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, RxData);
	   	    if (RxHeader.IDE == CAN_ID_STD && RxHeader.DLC == 1 && RxData[0] == 4)
	        {
	   	    	// Traitement des données
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
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
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

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
