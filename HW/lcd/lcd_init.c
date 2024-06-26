#include "lcd_init.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include <stdint.h>

void lcd_writ_bus(uint8_t dat) {
  uint8_t i;
  LCD_CS_Clr();
  for (i = 0; i < 8; i++) {
    LCD_SCLK_Clr();
    if (dat & 0X80) {
      LCD_MOSI_Set();
    } else {
      LCD_MOSI_Clr();
    }
    LCD_SCLK_Set();
    dat <<= 1;
  }
  LCD_CS_Set();
}
void lcd_wr_data8(uint8_t dat) { lcd_writ_bus(dat); }

void lcd_wr_data(uint16_t dat) {
  lcd_writ_bus(dat >> 8);
  lcd_writ_bus(dat);
}
void lcd_wr_reg(uint8_t dat) {
  LCD_DC_Clr();
  lcd_writ_bus(dat);
  LCD_DC_Set();
}
void lcd_address_set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
  if (USE_HORIZONTAL == 0) {
    lcd_wr_reg(0x2a); // 列地址设置
    lcd_wr_data(x1 + 24);
    lcd_wr_data(x2 + 24);
    lcd_wr_reg(0x2b); // 行地址设置
    lcd_wr_data(y1);
    lcd_wr_data(y2);
    lcd_wr_reg(0x2c); // 储存器写
  } else if (USE_HORIZONTAL == 1) {
    lcd_wr_reg(0x2a); // 列地址设置
    lcd_wr_data(x1 + 24);
    lcd_wr_data(x2 + 24);
    lcd_wr_reg(0x2b); // 行地址设置
    lcd_wr_data(y1);
    lcd_wr_data(y2);
    lcd_wr_reg(0x2c); // 储存器写
  } else if (USE_HORIZONTAL == 2) {
    lcd_wr_reg(0x2a); // 列地址设置
    lcd_wr_data(x1);
    lcd_wr_data(x2);
    lcd_wr_reg(0x2b); // 行地址设置
    lcd_wr_data(y1 + 24);
    lcd_wr_data(y2 + 24);
    lcd_wr_reg(0x2c); // 储存器写
  } else {
    lcd_wr_reg(0x2a); // 列地址设置
    lcd_wr_data(x1);
    lcd_wr_data(x2);
    lcd_wr_reg(0x2b); // 行地址设置
    lcd_wr_data(y1 + 24);
    lcd_wr_data(y2 + 24);
    lcd_wr_reg(0x2c); // 储存器写
  }
}

void lcd_init() {

  LCD_RES_Clr(); // 复位
  HAL_Delay(100);
  LCD_RES_Set();
  HAL_Delay(100);

  LCD_BLK_Set(); // 打开背光
  HAL_Delay(100);

  lcd_wr_reg(0x11); // Sleep exit
  HAL_Delay(120);   // Delay 120ms
  lcd_wr_reg(0xB1);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x3C);
  lcd_wr_data8(0x3C);

  lcd_wr_reg(0xB2);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x3C);
  lcd_wr_data8(0x3C);

  lcd_wr_reg(0xB3);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x3C);
  lcd_wr_data8(0x3C);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x3C);
  lcd_wr_data8(0x3C);

  lcd_wr_reg(0xB4); // Dot inversion
  lcd_wr_data8(0x03);

  lcd_wr_reg(0xC0);
  lcd_wr_data8(0x0E);
  lcd_wr_data8(0x0E);
  lcd_wr_data8(0x04);

  lcd_wr_reg(0xC1);
  lcd_wr_data8(0xC5);

  lcd_wr_reg(0xC2);
  lcd_wr_data8(0x0d);
  lcd_wr_data8(0x00);

  lcd_wr_reg(0xC3);
  lcd_wr_data8(0x8D);
  lcd_wr_data8(0x2A);

  lcd_wr_reg(0xC4);
  lcd_wr_data8(0x8D);
  lcd_wr_data8(0xEE);

  lcd_wr_reg(0xC5);   // VCOM
  lcd_wr_data8(0x06); // 1D  .06

  lcd_wr_reg(0x36); // MX, MY, RGB mode
  if (USE_HORIZONTAL == 0)
    lcd_wr_data8(0x08);
  else if (USE_HORIZONTAL == 1)
    lcd_wr_data8(0xC8);
  else if (USE_HORIZONTAL == 2)
    lcd_wr_data8(0x78);
  else
    lcd_wr_data8(0xA8);

  lcd_wr_reg(0x3A);
  lcd_wr_data8(0x55);

  lcd_wr_reg(0xE0);
  lcd_wr_data8(0x0b);
  lcd_wr_data8(0x17);
  lcd_wr_data8(0x0a);
  lcd_wr_data8(0x0d);
  lcd_wr_data8(0x1a);
  lcd_wr_data8(0x19);
  lcd_wr_data8(0x16);
  lcd_wr_data8(0x1d);
  lcd_wr_data8(0x21);
  lcd_wr_data8(0x26);
  lcd_wr_data8(0x37);
  lcd_wr_data8(0x3c);
  lcd_wr_data8(0x00);
  lcd_wr_data8(0x09);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x10);

  lcd_wr_reg(0xE1);
  lcd_wr_data8(0x0c);
  lcd_wr_data8(0x19);
  lcd_wr_data8(0x09);
  lcd_wr_data8(0x0d);
  lcd_wr_data8(0x1b);
  lcd_wr_data8(0x19);
  lcd_wr_data8(0x15);
  lcd_wr_data8(0x1d);
  lcd_wr_data8(0x21);
  lcd_wr_data8(0x26);
  lcd_wr_data8(0x39);
  lcd_wr_data8(0x3E);
  lcd_wr_data8(0x00);
  lcd_wr_data8(0x09);
  lcd_wr_data8(0x05);
  lcd_wr_data8(0x10);

  HAL_Delay(120);
  lcd_wr_reg(0x29); // Display on
  return;
}
