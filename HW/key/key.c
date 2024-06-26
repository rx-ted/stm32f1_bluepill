
#include "key.h"
#include "stm32f103xb.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"

#include <stdint.h>

uint8_t key_status() {
  uint8_t bit = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
  return bit;
}
