#include "usart.h"
#include "stdio.h"
#include "stm32f1xx.h"
#include <main.h>

UART_HandleTypeDef huart1;

int _write(int fd, char *ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == 1 || fd == 2) {
    hstatus = HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return -1;
  }
  return -1;
}

void Usart1_SendString(unsigned char *str) {}
void Usart1_SendPackage(unsigned char *data, unsigned short len) {}
void USART1_IRQHandler(void) {}

void Usart1_Init(unsigned int bound) {
  huart1.Instance = USART1;
  huart1.Init.BaudRate = bound;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
}
