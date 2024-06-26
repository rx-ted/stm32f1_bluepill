#ifndef __USART_H
#define __USART_H

#define USART1_RX_BUFF_MAX 256

typedef struct {
  unsigned char RxBuff[USART1_RX_BUFF_MAX];
  unsigned char RecFlag;
  unsigned short RecLen;
} USART1_TypeDef;

void Usart1_Init(unsigned int bound);
void Usart1_SendString(unsigned char *str);
void Usart1_SendPackage(unsigned char *data, unsigned short len);

#endif
