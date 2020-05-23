/* USER CODE BEGIN Header */
/*
 * Copyright  Michał Młodawski (SimpleMethod)(c) 2019.
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

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
#define ADC_SOIL_MISTURE_Pin GPIO_PIN_1
#define ADC_SOIL_MISTURE_GPIO_Port GPIOA
#define UART2_SDS011_TX_Pin GPIO_PIN_2
#define UART2_SDS011_TX_GPIO_Port GPIOA
#define UART2_SDS011_RX_Pin GPIO_PIN_3
#define UART2_SDS011_RX_GPIO_Port GPIOA
#define ADC_WIND_DIRECTION_Pin GPIO_PIN_4
#define ADC_WIND_DIRECTION_GPIO_Port GPIOA
#define UART1_ESP_TX_Pin GPIO_PIN_9
#define UART1_ESP_TX_GPIO_Port GPIOA
#define UART1_ESP_RX_Pin GPIO_PIN_10
#define UART1_ESP_RX_GPIO_Port GPIOA
#define I2CL_BME_SCL_Pin GPIO_PIN_6
#define I2CL_BME_SCL_GPIO_Port GPIOB
#define I2C1_BME_SDA_Pin GPIO_PIN_7
#define I2C1_BME_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
