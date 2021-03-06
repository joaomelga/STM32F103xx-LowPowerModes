/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 *  main.c
 *  Copyright (C) 2020  Marcelo Braga & Jo?o Melga
 *  Created on: mar-03-2021
 *  Institution: Universidade Federal de Minas Gerais - UFMG
 *
 *  This is a demo application about low power modes available in STM32F1 family
 *
 *  This API was developed as a work of the Embedded Systems Programming
 *  discipline from UFMG - Prof. Ricardo de Oliveira Duarte - Department
 *  of Electronic Engineering.
 *
 *  For more information about low power modes, please check the Application Note
 *  provided in this project repository
 *
 *  /--- LICENCE ---/
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  /--- SOFTWARE & HARDWARE REQUIREMENT ---/
 *  Please check the topic "3. Low power modes application example" of the
 *  Application Note provided in this project repository
 *
 *  /--- REFERENCES ---/
 *  - https://github.com/joaomelga/STM32F103xx-LowPowerModes
 *  - https://www.st.com/resource/en/application_note/dm00237631-stm32-power-mode-examples-stmicroelectronics.pdf
 *  - https://www.st.com/resource/en/application_note/cd00171691-stm32f101xx-stm32f102xx-and-stm32f103xx-lowpower-modes-stmicroelectronics.pdf
 *  - STM32F103xx Reference Manual - RM0008. Available on: www.st.com
 *  - https://controllerstech.com/low-power-modes-in-stm32
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define SLEEP_TESTING_STATE 0
#define STOP_TESTING_STATE 1
#define STANDBY_TESTING_STATE 2

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
RTC_HandleTypeDef hrtc;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
char *str;
uint8_t Rx_data;
uint8_t state;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void GPIO_Blink(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint8_t times,
    uint32_t delay) {
  for (uint8_t i = 0; i < times; i++) {
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    HAL_Delay(delay);
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
    HAL_Delay(delay);
  }

  HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  // In SLEEP and STOP testing states, turning off SleepOnExit will make main() code run again
  // Otherwise, the interruption would be executed and the MCU would sleep without look to main()
  switch (state) {
  case SLEEP_TESTING_STATE:
    if (GPIO_Pin == SLEEP_ON_EXIT_OFF_Pin)
      HAL_PWR_DisableSleepOnExit();
    else
      // LED_GREEN will be toggled = System has just woken up from SLEEP mode and SleepOnExit stills enabled
      HAL_GPIO_TogglePin(GPIOA, LED_GREEN_Pin);

    break;

  case STOP_TESTING_STATE:
    if (GPIO_Pin == SLEEP_ON_EXIT_OFF_Pin)
      HAL_PWR_DisableSleepOnExit();
    else
      // LED_RED will be toggled = System has just woken up from STOP mode and SleepOnExit stills enabled
      HAL_GPIO_TogglePin(GPIOA, LED_RED_Pin);

    break;

  case STANDBY_TESTING_STATE:
    if (GPIO_Pin == SLEEP_ON_EXIT_OFF_Pin)
      state = SLEEP_TESTING_STATE; // Changing the state is the way we avoid STANDBY to be performed again

    break;
  }
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
  MX_RTC_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET) {
    // If PWR_FLAG_SB was not RESET, system has been waked up in STANDBY mode
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);

    state = STANDBY_TESTING_STATE;
    // LED_RED will blink once very slowly = System has been woken up from STANDBY mode
    GPIO_Blink(GPIOA, LED_RED_Pin, 1, 3000);
  }

  else {
    // LED_GREEN will blink once very slowly = System has been powered on or reseted
    GPIO_Blink(GPIOA, LED_GREEN_Pin, 1, 3000);
    state = SLEEP_TESTING_STATE;
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    HAL_GPIO_WritePin(GPIOA, LED_GREEN_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, LED_RED_Pin, GPIO_PIN_RESET);

    switch (state) {
    case SLEEP_TESTING_STATE:
      // LED_GREEN will blink 3 times = System is about to enter in SLEEP mode and SleepOnExit is enabled
      GPIO_Blink(GPIOA, LED_GREEN_Pin, 3, 500);

      HAL_SuspendTick();
      HAL_PWR_EnableSleepOnExit();

      HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);

      HAL_ResumeTick();

      // LED_GREEN will blink 15 times quickly = System has just woken up from SLEEP mode and SleepOnExit is disabled
      GPIO_Blink(GPIOA, LED_GREEN_Pin, 15, 100);
      state = STOP_TESTING_STATE;
      break;

    case STOP_TESTING_STATE:
      // LED_RED will blink 3 times = System is about to enter in STOP mode and SleepOnExit is enabled
      GPIO_Blink(GPIOA, LED_RED_Pin, 3, 500);

      HAL_SuspendTick();
      HAL_PWR_EnableSleepOnExit();

      HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);

      SystemClock_Config(); // SystemClock may be configured again, because it was disabled
      HAL_ResumeTick();

      // LED_RED will blink 15 times quickly = System has just woken up from STOP mode and SleepOnExit is disabled
      GPIO_Blink(GPIOA, LED_RED_Pin, 15, 100);
      state = STANDBY_TESTING_STATE;
      break;

    case STANDBY_TESTING_STATE:
      __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
      HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

      HAL_PWR_EnterSTANDBYMode();
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
  RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI
      | RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
      | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief RTC Initialization Function
 * @param None
 * @retval None
 */
static void MX_RTC_Init(void) {

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only
   */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = { 0 };

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOD_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOA_CLK_ENABLE()
  ;
  __HAL_RCC_GPIOB_CLK_ENABLE()
  ;

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RED_Pin | LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_RED_Pin LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin | LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SLEEP_ON_EXIT_OFF_Pin */
  GPIO_InitStruct.Pin = SLEEP_ON_EXIT_OFF_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(SLEEP_ON_EXIT_OFF_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OTHER_BTN_Pin */
  GPIO_InitStruct.Pin = OTHER_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(OTHER_BTN_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
