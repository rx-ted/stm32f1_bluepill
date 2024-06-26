
#include "led.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include <stm32f1xx.h>

// PC13 - vcc
void led_off() {
  /*GPIOC->BSRR = GPIO_Pin_13;*/
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void led_on() {
  /*GPIOC->BRR = GPIO_Pin_13;*/
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}
