/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
#define LCD_CS_Pin GPIO_PIN_6
#define LCD_CS_GPIO_Port GPIOB
#define TLV5618_CS_Pin GPIO_PIN_9
#define TLV5618_CS_GPIO_Port GPIOB
#define MDS560R_DIR_Pin GPIO_PIN_7
#define MDS560R_DIR_GPIO_Port GPIOC
#define KEY_1_Pin GPIO_PIN_4
#define KEY_1_GPIO_Port GPIOA
#define KEY_2_Pin GPIO_PIN_6
#define KEY_2_GPIO_Port GPIOA
#define KEY_3_Pin GPIO_PIN_0
#define KEY_3_GPIO_Port GPIOB
#define KEY_4_Pin GPIO_PIN_2
#define KEY_4_GPIO_Port GPIOB
#define TEST_LED_Pin GPIO_PIN_15
#define TEST_LED_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_7
#define LED2_GPIO_Port GPIOA
#define LED3_Pin GPIO_PIN_1
#define LED3_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_10
#define LED4_GPIO_Port GPIOB
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
