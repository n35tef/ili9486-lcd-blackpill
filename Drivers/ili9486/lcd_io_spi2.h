#ifndef ILI9486_LCD_IO_SPI2_H
#define ILI9486_LCD_IO_SPI2_H

#include <stddef.h>
#include <stdint.h>

/* Low level IO layer: drives LCD_RST/LCD_DATA_CTL/SPI2_CS and SPI2 (see main.h for pin defs) */

/* Waveshare dtbo sets regwidth=16: fbtft sends each command/parameter as a 16-bit big-endian
   value (0x00 high byte) over the 8-bit bus. Set to 1 to match that framing. */
#define LCD_REGWIDTH16 1

void lcd_io_reset(void);
void lcd_io_write_cmd(uint8_t cmd);
void lcd_io_write_data(uint8_t data);
void lcd_io_write_data_buf(const uint8_t* buf, size_t len);
void lcd_io_write_pixels(const uint8_t* buf, size_t len);

/* Async variant: starts DMA and returns; caller must wait for lcd_io_pixels_async_done() */
void lcd_io_write_pixels_async(const uint8_t* buf, size_t len);
void lcd_io_pixels_async_done(void);

#endif /* ILI9486_LCD_IO_SPI2_H */
