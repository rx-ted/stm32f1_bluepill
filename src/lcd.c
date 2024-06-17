#include "lcd.h"
#include "delay.h"
#include "lcd_init.h"
#include "lcdfont.h"
#include <stdlib.h>

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,
              uint16_t color) {
  uint16_t i, j;
  LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
  for (i = ysta; i < yend; i++) {
    for (j = xsta; j < xend; j++) {
      LCD_WR_DATA(color);
    }
  }
}

void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t color) {
  LCD_Address_Set(x, y, x, y); // 设置光标位置
  LCD_WR_DATA(color);
}

void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
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
    LCD_DrawPoint(uRow, uCol, color); // 画点
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

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                       uint16_t color) {
  LCD_DrawLine(x1, y1, x2, y1, color);
  LCD_DrawLine(x1, y1, x1, y2, color);
  LCD_DrawLine(x1, y2, x2, y2, color);
  LCD_DrawLine(x2, y1, x2, y2, color);
}

void Draw_Circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color) {
  int a, b;
  a = 0;
  b = r;
  while (a <= b) {
    LCD_DrawPoint(x0 - b, y0 - a, color); // 3
    LCD_DrawPoint(x0 + b, y0 - a, color); // 0
    LCD_DrawPoint(x0 - a, y0 + b, color); // 1
    LCD_DrawPoint(x0 - a, y0 - b, color); // 2
    LCD_DrawPoint(x0 + b, y0 + a, color); // 4
    LCD_DrawPoint(x0 + a, y0 - b, color); // 5
    LCD_DrawPoint(x0 + a, y0 + b, color); // 6
    LCD_DrawPoint(x0 - b, y0 + a, color); // 7
    a++;
    if ((a * a + b * b) > (r * r)) // 判断要画的点是否过远
    {
      b--;
    }
  }
}

void LCD_ShowChinese(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                     uint16_t bc, uint8_t sizey, uint8_t mode) {
  while (*s != 0) {
    if (sizey == 12)
      LCD_ShowChinese12x12(x, y, s, fc, bc, sizey, mode);
    else if (sizey == 16)
      LCD_ShowChinese16x16(x, y, s, fc, bc, sizey, mode);
    else if (sizey == 24)
      LCD_ShowChinese24x24(x, y, s, fc, bc, sizey, mode);
    else if (sizey == 32)
      LCD_ShowChinese32x32(x, y, s, fc, bc, sizey, mode);
    else
      return;
    s += 2;
    x += sizey;
  }
}

void LCD_ShowChinese12x12(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                          uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;       // 汉字数目
  uint16_t TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

  HZnum = sizeof(tfont12) / sizeof(typFNT_GB12); // 统计汉字数目
  for (k = 0; k < HZnum; k++) {
    if ((tfont12[k].Index[0] == *(s)) && (tfont12[k].Index[1] == *(s + 1))) {
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for (i = 0; i < TypefaceNum; i++) {
        for (j = 0; j < 8; j++) {
          if (!mode) // 非叠加方式
          {
            if (tfont12[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else
              LCD_WR_DATA(bc);
            m++;
            if (m % sizey == 0) {
              m = 0;
              break;
            }
          } else // 叠加方式
          {
            if (tfont12[k].Msk[i] & (0x01 << j))
              LCD_DrawPoint(x, y, fc); // 画一个点
            x++;
            if ((x - x0) == sizey) {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }
    continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
  }
}

void LCD_ShowChinese16x16(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                          uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;       // 汉字数目
  uint16_t TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont16) / sizeof(typFNT_GB16); // 统计汉字数目
  for (k = 0; k < HZnum; k++) {
    if ((tfont16[k].Index[0] == *(s)) && (tfont16[k].Index[1] == *(s + 1))) {
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for (i = 0; i < TypefaceNum; i++) {
        for (j = 0; j < 8; j++) {
          if (!mode) // 非叠加方式
          {
            if (tfont16[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else
              LCD_WR_DATA(bc);
            m++;
            if (m % sizey == 0) {
              m = 0;
              break;
            }
          } else // 叠加方式
          {
            if (tfont16[k].Msk[i] & (0x01 << j))
              LCD_DrawPoint(x, y, fc); // 画一个点
            x++;
            if ((x - x0) == sizey) {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }
    continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
  }
}

void LCD_ShowChinese24x24(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                          uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;       // 汉字数目
  uint16_t TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont24) / sizeof(typFNT_GB24); // 统计汉字数目
  for (k = 0; k < HZnum; k++) {
    if ((tfont24[k].Index[0] == *(s)) && (tfont24[k].Index[1] == *(s + 1))) {
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for (i = 0; i < TypefaceNum; i++) {
        for (j = 0; j < 8; j++) {
          if (!mode) // 非叠加方式
          {
            if (tfont24[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else
              LCD_WR_DATA(bc);
            m++;
            if (m % sizey == 0) {
              m = 0;
              break;
            }
          } else // 叠加方式
          {
            if (tfont24[k].Msk[i] & (0x01 << j))
              LCD_DrawPoint(x, y, fc); // 画一个点
            x++;
            if ((x - x0) == sizey) {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }
    continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
  }
}

void LCD_ShowChinese32x32(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                          uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t i, j, m = 0;
  uint16_t k;
  uint16_t HZnum;       // 汉字数目
  uint16_t TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
  HZnum = sizeof(tfont32) / sizeof(typFNT_GB32); // 统计汉字数目
  for (k = 0; k < HZnum; k++) {
    if ((tfont32[k].Index[0] == *(s)) && (tfont32[k].Index[1] == *(s + 1))) {
      LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
      for (i = 0; i < TypefaceNum; i++) {
        for (j = 0; j < 8; j++) {
          if (!mode) // 非叠加方式
          {
            if (tfont32[k].Msk[i] & (0x01 << j))
              LCD_WR_DATA(fc);
            else
              LCD_WR_DATA(bc);
            m++;
            if (m % sizey == 0) {
              m = 0;
              break;
            }
          } else // 叠加方式
          {
            if (tfont32[k].Msk[i] & (0x01 << j))
              LCD_DrawPoint(x, y, fc); // 画一个点
            x++;
            if ((x - x0) == sizey) {
              x = x0;
              y++;
              break;
            }
          }
        }
      }
    }
    continue; // 查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
  }
}

void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc,
                  uint8_t sizey, uint8_t mode) {
  uint8_t temp, sizex, t, m = 0;
  uint16_t i, TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
  num = num - ' ';                                     // 得到偏移后的值
  LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
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
          LCD_WR_DATA(fc);
        else
          LCD_WR_DATA(bc);
        m++;
        if (m % sizex == 0) {
          m = 0;
          break;
        }
      } else // 叠加模式
      {
        if (temp & (0x01 << t))
          LCD_DrawPoint(x, y, fc); // 画一个点
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

void LCD_ShowString(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc,
                    uint16_t bc, uint8_t sizey, uint8_t mode) {
  while (*p != '\0') {
    LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
    x += sizey / 2;
    p++;
  }
}

uint32_t mypow(uint8_t m, uint8_t n) {
  uint32_t result = 1;
  while (n--)
    result *= m;
  return result;
}

void LCD_ShowIntNum(uint16_t x, uint16_t y, uint16_t num, uint8_t len,
                    uint16_t fc, uint16_t bc, uint8_t sizey) {
  uint8_t t, temp;
  uint8_t enshow = 0;
  uint8_t sizex = sizey / 2;
  for (t = 0; t < len; t++) {
    temp = (num / mypow(10, len - t - 1)) % 10;
    if (enshow == 0 && t < (len - 1)) {
      if (temp == 0) {
        LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
        continue;
      } else
        enshow = 1;
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}

void LCD_ShowFloatNum1(uint16_t x, uint16_t y, float num, uint8_t len,
                       uint16_t fc, uint16_t bc, uint8_t sizey) {
  uint8_t t, temp, sizex;
  uint16_t num1;
  sizex = sizey / 2;
  num1 = num * 100;
  for (t = 0; t < len; t++) {
    temp = (num1 / mypow(10, len - t - 1)) % 10;
    if (t == (len - 2)) {
      LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
      t++;
      len += 1;
    }
    LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
  }
}

void LCD_ShowPicture(uint16_t x, uint16_t y, uint16_t length, uint16_t width,
                     const uint8_t pic[]) {
  uint16_t i, j;
  uint32_t k = 0;
  LCD_Address_Set(x, y, x + length - 1, y + width - 1);
  for (i = 0; i < length; i++) {
    for (j = 0; j < width; j++) {
      LCD_WR_DATA8(pic[k * 2]);
      LCD_WR_DATA8(pic[k * 2 + 1]);
      k++;
    }
  }
}

void LCD_ShowFlashString(uint16_t x, uint16_t y, char *str, uint16_t fc,
                         uint16_t bc, uint8_t sizey, uint8_t mode) {
  while (*str != 0) {
    if (*str < 127) { // ASCII可见字符最大为126

      LCD_ShowFlashChar(x, y, *str, fc, bc, sizey, mode); // 显示字符
      x += (sizey / 2);                                   // 列地址偏移
      str++;
      if (x > 160 - (sizey / 2)) { // 如果走到了边沿
        x = 0;                     // 回到首列
        y += sizey;                // 行地址偏移
      }
    } else {
      LCD_ShowFlashGB2312(x, y, (uint8_t *)str, fc, bc, sizey,
                          mode); // 显示中文
      x += sizey;                // 列地址偏移
      str += 2;                  // 一个中文两个字节
      if (x > 160 - sizey) {     // 如果走到了边沿
        x = 0;                   // 回到首列
        y += sizey;              // 行地址偏移
      }
      if (y >= 80)
        return;
    }
  }
}

void LCD_ShowFlashGB2312(uint16_t x, uint16_t y, uint8_t *s, uint16_t fc,
                         uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t i, j, m = 0;
  uint16_t TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  TypefaceNum = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;

  unsigned char GB2312[TypefaceNum];
  memset(GB2312, 0, TypefaceNum);
  unsigned char c = 0;

  switch (sizey) {
  case 12:
    W25Qxx_ReadDatas(GB2312,
                     GB2312_12X12 +
                         ((*s - 0xA1) * 94 + (*(s + 1) - 0xA1)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 16:
    W25Qxx_ReadDatas(GB2312,
                     GB2312_16X16 +
                         ((*s - 0xA1) * 94 + (*(s + 1) - 0xA1)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 24:
    W25Qxx_ReadDatas(GB2312,
                     GB2312_24X24 +
                         ((*s - 0xA1) * 94 + (*(s + 1) - 0xA1)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 32:
    W25Qxx_ReadDatas(GB2312,
                     GB2312_32X32 +
                         ((*s - 0xA1) * 94 + (*(s + 1) - 0xA1)) * TypefaceNum,
                     TypefaceNum);
    break;
  }

  LCD_Address_Set(x, y, x + sizey - 1, y + sizey - 1);
  for (i = 0; i < TypefaceNum; i++) {
    c = GB2312[i];
    for (j = 0; j < 8; j++) {
      if (!mode) // 非叠加方式
      {
        if (c & (0x01 << j))
          LCD_WR_DATA(fc);
        else
          LCD_WR_DATA(bc);
        m++;
        if (m % sizey == 0) {
          m = 0;
          break;
        }
      } else // 叠加方式
      {
        if (c & (0x01 << j))
          LCD_DrawPoint(x, y, fc); // 画一个点
        x++;
        if ((x - x0) == sizey) {
          x = x0;
          y++;
          break;
        }
      }
    }
  }
}

void LCD_ShowFlashChar(uint16_t x, uint16_t y, uint8_t num, uint16_t fc,
                       uint16_t bc, uint8_t sizey, uint8_t mode) {
  uint8_t temp, sizex, t, m = 0;

  uint16_t i, TypefaceNum; // 一个字符所占字节大小
  uint16_t x0 = x;
  sizex = sizey / 2;
  TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
  uint8_t ACSII[TypefaceNum];
  memset(ACSII, 0, TypefaceNum);

  switch (sizey) {
  case 12:
    W25Qxx_ReadDatas(ACSII, Acsii_1206 + ((num - 32)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 16:
    W25Qxx_ReadDatas(ACSII, Acsii_1608 + ((num - 32)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 24:
    W25Qxx_ReadDatas(ACSII, Acsii_2412 + ((num - 32)) * TypefaceNum,
                     TypefaceNum);
    break;
  case 32:
    W25Qxx_ReadDatas(ACSII, Acsii_3212 + ((num - 32)) * TypefaceNum,
                     TypefaceNum);
    break;
  }

  LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); // 设置光标位置
  for (i = 0; i < TypefaceNum; i++) {
    temp = ACSII[i];
    for (t = 0; t < 8; t++) {
      if (!mode) // 非叠加模式
      {
        if (temp & (0x01 << t))
          LCD_WR_DATA(fc);
        else
          LCD_WR_DATA(bc);
        m++;
        if (m % sizex == 0) {
          m = 0;
          break;
        }
      } else // 叠加模式
      {
        if (temp & (0x01 << t))
          LCD_DrawPoint(x, y, fc); // 画一个点
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
