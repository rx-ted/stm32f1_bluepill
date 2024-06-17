#include "usart.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include <stdint.h>

USART1_TypeDef usart1;

void Usart1_SendString(unsigned char *str) {
  while (*str != 0) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
      ;                             // 等待上一个字节发送完成
    USART_SendData(USART1, *str++); // 发送一个字节
  }
}
void Usart1_SendPackage(unsigned char *data, unsigned short len) {
  while (len--) {
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
      ;                              // 等待上一个字节发送完成
    USART_SendData(USART1, *data++); // 发送一个字节
  }
}
void USART1_IRQHandler(void) {
  if (USART_GetITStatus(USART1, USART_IT_RXNE) ==
      SET) // 如果收到了数据（判断触发了什么中断）
  {
    usart1.RxBuff[usart1.RecLen] = USART_ReceiveData(USART1); // 读取数据
    if (++usart1.RecLen >
        USART1_RX_BUFF_MAX) // 如果接收到的数据超过缓冲区大小，那么从头开始存放
      usart1.RecLen = 0;
  } else if (USART_GetITStatus(USART1, USART_IT_IDLE) ==
             SET) // 如果触发了空闲中断，那么字符串接收完成
  {
    usart1.RxBuff[usart1.RecLen] = 0; // 在当前的末尾添加结束符
    usart1.RecFlag = 1;        // 标示位置1，通知主程序进行处理
    USART_ReceiveData(USART1); // 读取DR会对SR清零
  }
}
void Usart1_Init(unsigned int bound) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  USART_InitTypeDef USART_InitStruct = {0};

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);

  USART_InitStruct.USART_BaudRate = bound; // 波特率
  USART_InitStruct.USART_HardwareFlowControl =
      USART_HardwareFlowControl_None; // 不使用硬件控制流
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 收发模式
  USART_InitStruct.USART_Parity = USART_Parity_No;    // 不使用奇偶校验
  USART_InitStruct.USART_StopBits = USART_StopBits_1; // 一位停止位
  USART_InitStruct.USART_WordLength = USART_WordLength_8b; // 八位的数据长度
  USART_Init(USART1, &USART_InitStruct);

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接收中断
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE); // 使能空闲中断

  //	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  //	NVIC_InitStruct.NVIC_IRQChannel    = USART1_IRQn;		//中断源
  //	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
  ////使能中断源 	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
  ////抢占优先级2 	NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 2;
  ////响应优先级2 	NVIC_Init(&NVIC_InitStruct);

  USART_Cmd(USART1, ENABLE); // 使能外设
}

/**************************************************************************************************************/

/*****加入以下代码,支持printf函数,而不需要选择use MicroLIB*****/
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE {
  int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x) { x = x; }
void _ttywrch(int ch) { ch = ch; }
// 重定义fputc函数
int fputc(int ch, FILE *f) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    ;
  USART_SendData(USART1, ch);
  return ch;
}
/**********************printf support end**********************/
#endif
