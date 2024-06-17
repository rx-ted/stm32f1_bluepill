
#include "key.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdint.h>

void key_init() {

  GPIO_InitTypeDef GPIO_InitStructure;

  /*RCC->APB2ENR |= 1 << 4; // ENABLE GPIOC clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  // LED=PC13
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t key_status() {
  uint8_t bit = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
  return bit;
}
