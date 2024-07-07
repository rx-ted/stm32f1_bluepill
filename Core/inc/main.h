
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

#define LED0_Pin GPIO_PIN_13
#define LED0_GPIO_Port GPIOC
#define SPI2_CS_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_12

#ifdef DEBUG
#define DBG(...)                                                               \
  printf(__VA_ARGS__);                                                         \
  printf("\n")
#else
#define DBG(...)
#endif

#ifdef __cplusplus
}
#endif

#endif
