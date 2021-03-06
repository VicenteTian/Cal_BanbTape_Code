/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define Buzzer_Pin GPIO_PIN_13
#define Buzzer_GPIO_Port GPIOC
#define SPI1_NSS_Pin GPIO_PIN_4
#define SPI1_NSS_GPIO_Port GPIOA
#define O1_Pin GPIO_PIN_11
#define O1_GPIO_Port GPIOB
#define O2_Pin GPIO_PIN_12
#define O2_GPIO_Port GPIOB
#define O3_Pin GPIO_PIN_13
#define O3_GPIO_Port GPIOB
#define O4_Pin GPIO_PIN_14
#define O4_GPIO_Port GPIOB
#define I1_Pin GPIO_PIN_15
#define I1_GPIO_Port GPIOB
#define I2_Pin GPIO_PIN_8
#define I2_GPIO_Port GPIOA
#define I3_Pin GPIO_PIN_9
#define I3_GPIO_Port GPIOA
#define I4_Pin GPIO_PIN_10
#define I4_GPIO_Port GPIOA
#define P_State_Pin GPIO_PIN_4
#define P_State_GPIO_Port GPIOB
#define P_Hold_Pin GPIO_PIN_5
#define P_Hold_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
