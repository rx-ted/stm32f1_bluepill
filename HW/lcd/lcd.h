#ifndef __LCD_H
#define __LCD_H
// 画笔颜色
#include <stdint.h>
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0XF81F
#define GRED 0XFFE0
#define GBLUE 0X07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0XBC40
#define BRRED 0XFC07
#define GRAY 0X8430
#define DARKBLUE 0X01CF
#define LIGHTBLUE 0X7D7C
#define GRAYBLUE 0X5458
#define LIGHTGREEN 0X841F
#define LGRAY 0XC618
#define LGRAYBLUE 0XA651
#define LBBLUE 0X2B12

void lcd_draw_array(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                    uint16_t color);
void lcd_fill(uint16_t color);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                   uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                        uint16_t color);
void lcd_draw_circle(uint16_t x0, uint16_t y0, uint8_t r, uint16_t color);
void lcd_show_char(uint16_t x, uint16_t y, uint8_t num, uint16_t fc,
                   uint16_t bc, uint8_t sizey, uint8_t mode);
void lcd_show_string(uint16_t x, uint16_t y, const uint8_t *p, uint16_t fc,
                     uint16_t bc, uint8_t sizey, uint8_t mode);
#endif
