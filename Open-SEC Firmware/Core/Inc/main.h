/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern FDCAN_HandleTypeDef hfdcan2;

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
bool mainInitAfterConfig(void);
bool mainHaltRisky(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CONTOLLOOP_DIV 16
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOC
#define LED1_Pin GPIO_PIN_14
#define LED1_GPIO_Port GPIOC
#define LED0_Pin GPIO_PIN_15
#define LED0_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_10
#define RST_GPIO_Port GPIOG
#define VLS_Pin GPIO_PIN_1
#define VLS_GPIO_Port GPIOA
#define IHS__Pin GPIO_PIN_2
#define IHS__GPIO_Port GPIOA
#define IHS_A3_Pin GPIO_PIN_3
#define IHS_A3_GPIO_Port GPIOA
#define VHS_Pin GPIO_PIN_7
#define VHS_GPIO_Port GPIOA
#define ID0_Pin GPIO_PIN_4
#define ID0_GPIO_Port GPIOC
#define ID1_Pin GPIO_PIN_0
#define ID1_GPIO_Port GPIOB
#define ID2_Pin GPIO_PIN_1
#define ID2_GPIO_Port GPIOB
#define ID3_Pin GPIO_PIN_2
#define ID3_GPIO_Port GPIOB
#define ILS__Pin GPIO_PIN_14
#define ILS__GPIO_Port GPIOB
#define ILS_B15_Pin GPIO_PIN_15
#define ILS_B15_GPIO_Port GPIOB
#define Tamb_Pin GPIO_PIN_8
#define Tamb_GPIO_Port GPIOA
#define Tsink_Pin GPIO_PIN_9
#define Tsink_GPIO_Port GPIOA
#define P2_PWM___LS_Pin GPIO_PIN_10
#define P2_PWM___LS_GPIO_Port GPIOA
#define P2_EN___HS_Pin GPIO_PIN_11
#define P2_EN___HS_GPIO_Port GPIOA
#define PHSEN_Pin GPIO_PIN_12
#define PHSEN_GPIO_Port GPIOA
#define PEN_Pin GPIO_PIN_15
#define PEN_GPIO_Port GPIOA
#define DREN_Pin GPIO_PIN_4
#define DREN_GPIO_Port GPIOB
#define VDRVPG_Pin GPIO_PIN_7
#define VDRVPG_GPIO_Port GPIOB
#define BOOT0_Pin GPIO_PIN_8
#define BOOT0_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_9
#define LED3_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define HWVersion_0_2 /* Define this with the newer hardware */
#define HW_NAME "Reboost"
#define FW_VERSION_MAJOR		0
#define FW_VERSION_MINOR		2
#define FW_REAL_VERSION			"V0.2"
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
