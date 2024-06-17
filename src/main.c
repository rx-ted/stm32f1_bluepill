#include "key.h"
#include "lcd.h"
#include "lcd_init.h"
#include "led.h"
#include "stdio.h"
#include "w25qxx.h"
#include <delay.h>
/*#include <lcdfont.h>*/
#include <stdint.h>
#include <usart.h>

typedef enum { FAILED = 0, PASSED = !FAILED } TestStatus;

/* 获取缓冲区的长度 */
#define TxBufferSize1 (countof(TxBuffer1) - 1)
#define RxBufferSize1 (countof(TxBuffer1) - 1)
#define countof(a) (sizeof(a) / sizeof(*(a)))
#define BufferSize (countof(Tx_Buffer) - 1)

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

#define Erase 0
uint8_t Tx_Buffer[] = "123456";

uint8_t Rx_Buffer[BufferSize];
__IO uint32_t DeviceID = 0;
__IO uint32_t FlashID = 0;
__IO TestStatus TransferStatus1 = FAILED;

// 函数原型声明
TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2,
                     uint16_t BufferLength);

int main(void) {
  float t = 0;
  __IO uint32_t Flash_Size = 0;

  unsigned char *str = "Hello Ben!\n";
  SystemInit();
  led_init();
  key_init();
  delay_init();
  Usart1_Init(115200);
  SPI_FLASH_Init();
  LCD_Init();
  printf("%s", str);
  LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);

  led_on();

  /* 获取 Flash Device ID */
  /*DeviceID = SPI_FLASH_ReadDeviceID();*/
  /*获取 SPI Flash ID */
  /*FlashID = SPI_FLASH_ReadID();*/
  /*printf("\r\n FlashID is 0x%X \r\n Device ID is 0x%X\r\n", FlashID,
     DeviceID);*/

  /*/* 检验 SPI Flash ID */
  /*if (FlashID == W25Q16ID || W25Q32ID || W25Q64ID || W25Q80ID) {*/
  /**/
  /*  printf("\r\n 检测到串行flash芯片!\r\n");*/
  /*  printf("\r\n Check flash id: ");*/
  /*  switch (FlashID) {*/
  /*  case W25Q16ID:*/
  /*    printf("W25Q16ID!\r\n");*/
  /*    Flash_Size = 2;*/
  /*    break;*/
  /*  case W25Q32ID:*/
  /*    printf("W25Q32!\r\n");*/
  /*    Flash_Size = 4;*/
  /*    break;*/
  /*  case W25Q64ID:*/
  /*    printf("W25Q64!\r\n");*/
  /*    Flash_Size = 8;*/
  /*    break;*/
  /*  case W25Q80ID:*/
  /*    printf("W25Q80!\r\n");*/
  /*    Flash_Size = 1;*/
  /*    break;*/
  /*  default:*/
  /*    printf("...Not found ID!\r\n");*/
  /*    Flash_Size = 0;*/
  /*    break;*/
  /*  }*/
  /**/
  /*  /* 擦除将要写入的 SPI FLASH 扇区，FLASH写入前要先擦除 */
  /*  // 这里擦除4K，即一个扇区，擦除的最小单位是扇区*/
  /*  SPI_FLASH_SectorErase(FLASH_SectorToErase);*/
  /**/
  /*  /* 将发送缓冲区的数据写到flash中 */
  /*  // 这里写一页，一页的大小为256个字节*/
  /*  SPI_FLASH_BufferWrite(Tx_Buffer, FLASH_WriteAddress, BufferSize);*/
  /*  printf("\r\n write the data: %s \r\t", Tx_Buffer);*/
  /**/
  /*  /* 将刚刚写入的数据读出来放到接收缓冲区中 */
  /*  SPI_FLASH_BufferRead(Rx_Buffer, FLASH_ReadAddress, BufferSize);*/
  /*  printf("\r\n read the data: %s \r\n", Rx_Buffer);*/
  /**/
  /*  /* 检查写入的数据与读出的数据是否相等 */
  /*  TransferStatus1 = Buffercmp(Tx_Buffer, Rx_Buffer, BufferSize);*/
  /**/
  /*  if (PASSED == TransferStatus1) {*/
  /**/
  /*    printf("\r\n Check successfully!\n\r");*/
  /*  } else {*/
  /**/
  /*    printf("\r\n Check unsuccessfully!\n\r");*/
  /*  }*/
  /*} // if (FlashID == sFLASH_ID)*/
  /*else // if (FlashID == sFLASH_ID)*/
  /*{*/
  /*  printf("\r\n Not found W25Q64 ID!\n\r");*/
  /*}*/
  /**/
  /*while (1) {*/
  /*  // 屏幕内容显示*/
  /*  LCD_ShowString(40, 0, "ABROBOT", RED, WHITE, 16, 0);*/
  /*}*/
  delay_ms(500);
  led_off();
  delay_ms(500);
}

TestStatus Buffercmp(uint8_t *pBuffer1, uint8_t *pBuffer2,
                     uint16_t BufferLength) {
  while (BufferLength--) {
    if (*pBuffer1 != *pBuffer2) {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}
