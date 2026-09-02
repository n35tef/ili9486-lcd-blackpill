#ifndef ILI9486_LCD_IO_SPI2_H
#define ILI9486_LCD_IO_SPI2_H

#include <stddef.h>
#include <stdint.h>

#define LCD_REGWIDTH16 1

void lcd_io_reset(void);
void lcd_io_write_cmd(uint8_t cmd);
void lcd_io_write_data(uint8_t data);
void lcd_io_write_data_buf(const uint8_t* buf, size_t len);
void lcd_io_write_pixels(const uint8_t* buf, size_t len);

void lcd_io_write_pixels_async(const uint8_t* buf, size_t len);
void lcd_io_pixels_async_done(void);

#endif /* ILI9486_LCD_IO_SPI2_H */
