#include "lcd.h"
#include "lcd_init.h"
#include "lcdfont.h"
#include <stdint.h>

void lcd_draw_array(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                    uint16_t color) {
  uint16_t i, j;
  lcd_address_set(x1, y1, x2 - 1, y2 - 1);
  for (i = y1; i < y2; i++) {
    for (j = x1; j < x2; j++) {
      lcd_wr_data(color);
    }
  }
}
void lcd_fill(uint16_t color) { lcd_draw_array(0, 0, LCD_W, LCD_H, color); }

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color) {
  lcd_address_set(x, y, x + 1, y + 1);
  lcd_wr_data(color);
}
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint16_t color) {
  uint16_t t;
  int xerr = 0, yerr = 0, delta_x, delta_y, distance;
  int incx, incy, uRow, uCol;
  delta_x = x2 - x1; // 计算坐标增量
  delta_y = y2 - y1;
  uRow = x1; // 画线起点坐标
  uCol = y1;
  if (delta_x > 0)
    incx = 1; // 设置单步方向
  else if (delta_x == 0)
    incx = 0; // 垂直线
  else {
    incx = -1;
    delta_x = -delta_x;
  }
  if (delta_y > 0)
    incy = 1;
  else if (delta_y == 0)
    incy = 0; // 水平线
  else {
    incy = -1;
    delta_y = -delta_y;
  }
  if (delta_x > delta_y)
    distance = delta_x; // 选取基本增量坐标轴
  else
    distance = delta_y;
  for (t = 0; t < distance + 1; t++) {
    lcd_draw_point(uRow, uCol, color); // 画点
    xerr += delta_x;
    yerr += delta_y;
    if (xerr > distance) {
      xerr -= distance;
      uRow += incx;
    }
    if (yerr > distance) {
      yerr -= distance;
      uCol += incy;
    }
  }
}
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t color) {
  lcd_draw_line(x1, y1, x2, y1, color);
  lcd_draw_line(x1, y1, x1, y2, color);
  lcd_draw_line(x1, y2, x2, y2, color);
  lcd_draw_line(x2, y1, x2, y2, color);
}
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color) {
  int a, b;
  a = 0;
  b = r;
  while (a <= b) {
    lcd_draw_point(x0 - b, y0 - a, color); // 3
    lcd_draw_point(x0 + b, y0 - a, color); // 0
    lcd_draw_point(x0 - a, y0 + b, color); // 1
    lcd_draw_point(x0 - a, y0 - b, color); // 2
    lcd_draw_point(x0 + b, y0 + a, color); // 4
    lcd_draw_point(x0 + a, y0 - b, color); // 5
    lcd_draw_point(x0 + a, y0 + b, color); // 6
    lcd_draw_point(x0 - b, y0 + a, color); // 7
    a++;
    if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
    {
      b--;
    }
  }
}

void lcd_show_char(uint16_t x, uint16_t y, uint8_t num, uint16_t fc,
                   uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t temp, sizex, t, m = 0;
  uint16_t i, TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
  num = num - ' ';                                     // 得到偏移后的值
  lcd_address_set(x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
  for (i = 0; i < TypefaceNum; i++) {
    if (sizey == 12)
      temp = ascii_1206[num][i]; // 调用6x12字体
    else if (sizey == 16)
      temp = ascii_1608[num][i]; // 调用8x16字体
    else if (sizey == 24)
      temp = ascii_2412[num][i]; // 调用12x24字体
    else if (sizey == 32)
      temp = ascii_3216[num][i]; // 调用16x32字体
    else
      return;
    for (t = 0; t < 8; t++) {
      if (!mode) // 非叠加模式
      {
        if (temp & (0x01 << t))
          lcd_wr_data(fc);
        else
          lcd_wr_data(bc);
        m++;
        if (m % sizex == 0) {
          m = 0;
          break;
        }
      } else // 叠加模式
      {
        if (temp & (0x01 << t))
          lcd_draw_point(x, y, fc); // 画一个点
        x++;
        if ((x - x0) == sizex) {
          x = x0;
          y++;
          break;
        }
      }
    }
  }
}
void lcd_show_string(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc,
                     uint16_t bc, uint8_t sizey, uint8_t mode) {

  while (*p != '\0') {
    if (x >= LCD_W) {
      y += sizey;
      x = 0;
    }
    if (y >= LCD_H) {
      y = 0;
      x = 0;
    }

    lcd_show_char(x, y, *p, fc, bc, sizey, mode);
    x += sizey / 2;
    p++;
  }
}
