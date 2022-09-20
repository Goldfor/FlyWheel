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
#include "stm32f1xx_hal.h"

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

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PWM_MOTOR_Pin GPIO_PIN_0
#define PWM_MOTOR_GPIO_Port GPIOA
#define POLARITY_COIL_Pin GPIO_PIN_4
#define POLARITY_COIL_GPIO_Port GPIOA
#define ADC_ON_Pin GPIO_PIN_5
#define ADC_ON_GPIO_Port GPIOA
#define HALL2_Pin GPIO_PIN_6
#define HALL2_GPIO_Port GPIOA
#define C_PWR_Pin GPIO_PIN_10
#define C_PWR_GPIO_Port GPIOB
#define TEST1_Pin GPIO_PIN_11
#define TEST1_GPIO_Port GPIOB
#define Led_Pin GPIO_PIN_14
#define Led_GPIO_Port GPIOB
#define PWM_COIL_Pin GPIO_PIN_8
#define PWM_COIL_GPIO_Port GPIOA
#define ADDR0_Pin GPIO_PIN_4
#define ADDR0_GPIO_Port GPIOB
#define ADDR1_Pin GPIO_PIN_5
#define ADDR1_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_6
#define HALL1_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
