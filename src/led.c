
#include "led.h"

void led_init() {
  GPIO_InitTypeDef GPIO_InitStructure;

  /*RCC->APB2ENR |= 1 << 4; // ENABLE GPIOC clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /*GPIOC->CRH = 0x00300010;*/
  /*GPIOC->CRH &= 0xff0fffff;*/
  /*GPIOC->CRH |= 0x00300000;*/
  /*GPIOC->ODR = 1 << 13;*/

  // LED=PC13
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  led_off();

  return;
}

void led_off() {

  /*GPIOC->BSRR = GPIO_Pin_13;*/
  GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

void led_on() {
  /*GPIOC->BRR = GPIO_Pin_13;*/
  GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}
