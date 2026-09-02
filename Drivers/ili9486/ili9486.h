#ifndef ILI9486_H
#define ILI9486_H

#include <stdint.h>

#define ILI9486_WIDTH 480
#define ILI9486_HEIGHT 320
#define LCD_BYTES_PER_PIXEL 2

void ili9486_init(void);
void ili9486_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void ili9486_fill_screen(uint16_t color);

#define ILI9486_RED 0xF800
#define ILI9486_GREEN 0x07E0
#define ILI9486_BLUE 0x001F

#endif /* ILI9486_H */
