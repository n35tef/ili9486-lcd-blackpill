#ifndef ILI9486_H
#define ILI9486_H

#include <stdint.h>

/* Confirmed genuine ILI9486, driven with the vendor init sequence decoded from Waveshare's
   waveshare35c.dtbo (regwidth=16 SPI framing - see lcd_io_spi2.h LCD_REGWIDTH16). */

/* MADCTL=0xE8 (MY|MX|MV|BGR): landscape, 180° rotated to match physical panel mounting */
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
