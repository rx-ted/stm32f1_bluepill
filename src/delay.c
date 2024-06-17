#include "delay.h"
#include "misc.h"
#include <stdint.h>

static uint8_t fac_us = 0;
static uint16_t fac_ms = 0;

void delay_init() {
  // if there is 72MHz
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  // 72 000 000Hz / 8 000 000 Hz = 9Hz
  fac_us = SystemCoreClock / 8000000; // get the value is 9Hz
  fac_ms = fac_us * 1000;             // 9KHz
}

void delay_us(uint32_t us) {
  uint32_t tmp = 0;
  SysTick->LOAD = us * fac_us; // time load
  SysTick->VAL = 0x00;         // clear value =0
  SysTick->CTRL |= 0x01;       // enable compute
  do {
    tmp = SysTick->CTRL; // record
  } while (!(tmp & (1 << 16)));
  SysTick->CTRL &= 0x00; // close compute
  SysTick->VAL = 0x00;   // clear value
}

void delay_ms(uint32_t ms) {
  uint32_t tmp = 0;
  SysTick->LOAD = ms * fac_ms; // time load
  SysTick->VAL = 0x00;         // clear value =0
  SysTick->CTRL |= 0x01;       // enable compute
  do {
    tmp = SysTick->CTRL; // record
  } while (!(tmp & (1 << 16)));
  SysTick->CTRL &= 0x00; // close compute
  SysTick->VAL = 0x00;   // clear value
}
