
#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "stdint.h"
#include "stm32f1xx.h"
#define USE_HORIZONTAL 2 // 设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W 80
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 80
#endif

//-----------------LCD端口定义----------------

#define LCD_SCLK_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET)
#define LCD_SCLK_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET)
#define LCD_MOSI_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_MOSI_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET)
#define LCD_RES_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define LCD_RES_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)
#define LCD_DC_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET)
#define LCD_DC_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET)
#define LCD_CS_Clr() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)
#define LCD_CS_Set() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET)
#define LCD_BLK_Clr() ;
#define LCD_BLK_Set() ;

void lcd_writ_bus(uint8_t dat); // 模拟SPI时序
void lcd_wr_data8(uint8_t dat); // 写入一个字节
void lcd_wr_data(uint16_t dat); // 写入两个字节
void lcd_wr_reg(uint8_t dat);   // 写入一个指令
void lcd_address_set(uint16_t x1, uint16_t y1, uint16_t x2,
                     uint16_t y2); // 设置坐标函数
void lcd_init(void);               // LCD初始化
#endif
