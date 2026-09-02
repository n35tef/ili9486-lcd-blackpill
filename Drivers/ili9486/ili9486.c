#include "Drivers/ili9486/ili9486.h"
#include "Drivers/ili9486/lcd_io_spi2.h"
#include "main.h"

#define ILI9486_CMD_SLPOUT 0x11
#define ILI9486_CMD_DISPON 0x29
#define ILI9486_CMD_CASET 0x2A
#define ILI9486_CMD_PASET 0x2B
#define ILI9486_CMD_RAMWR 0x2C
#define ILI9486_CMD_MADCTL 0x36
#define ILI9486_CMD_PIXFMT 0x3A
#define ILI9486_CMD_PWCTR2 0xC1
#define ILI9486_CMD_VMCTR1 0xC5
#define ILI9486_CMD_PGAMCTRL 0xE0
#define ILI9486_CMD_NGAMCTRL 0xE1

#define WR_CMD(c) lcd_io_write_cmd((uint8_t)(c))
#define WR_DAT(d) lcd_io_write_data((uint8_t)(d))
#define WR_BUF(...)                                                                                \
    lcd_io_write_data_buf((const uint8_t[]){__VA_ARGS__}, sizeof((const uint8_t[]){__VA_ARGS__}))

void ili9486_init(void)
{
    lcd_io_reset();

    // Decoded from Waveshare's own waveshare35c.dtbo;
    WR_CMD(0xF1);
    WR_BUF(0x36, 0x04, 0x00, 0x3C, 0x0F, 0x8F);
    WR_CMD(0xF2);
    WR_BUF(0x18, 0xA3, 0x12, 0x02, 0xB2, 0x12, 0xFF, 0x10, 0x00);
    WR_CMD(0xF8);
    WR_BUF(0x21, 0x04);
    WR_CMD(0xF9);
    WR_BUF(0x00, 0x08);
    WR_CMD(ILI9486_CMD_MADCTL);
    WR_DAT(0x08);
    WR_CMD(0xB4);
    WR_DAT(0x00);
    WR_CMD(ILI9486_CMD_PWCTR2);
    WR_DAT(0x41);
    WR_CMD(ILI9486_CMD_VMCTR1);
    WR_BUF(0x00, 0x91, 0x80, 0x00);
    WR_CMD(ILI9486_CMD_PGAMCTRL);
    WR_BUF(0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D,
           0x00);
    WR_CMD(ILI9486_CMD_NGAMCTRL);
    WR_BUF(0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20,
           0x00);
    WR_CMD(ILI9486_CMD_PIXFMT);
    WR_DAT(0x55);
    WR_CMD(ILI9486_CMD_SLPOUT);
    WR_CMD(ILI9486_CMD_MADCTL);
    WR_DAT(0xE8);
    HAL_Delay(255);
    WR_CMD(ILI9486_CMD_DISPON);
}

void ili9486_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    lcd_io_write_cmd(ILI9486_CMD_CASET);
    lcd_io_write_data((uint8_t)(x0 >> 8));
    lcd_io_write_data((uint8_t)(x0 & 0xFF));
    lcd_io_write_data((uint8_t)(x1 >> 8));
    lcd_io_write_data((uint8_t)(x1 & 0xFF));

    lcd_io_write_cmd(ILI9486_CMD_PASET);
    lcd_io_write_data((uint8_t)(y0 >> 8));
    lcd_io_write_data((uint8_t)(y0 & 0xFF));
    lcd_io_write_data((uint8_t)(y1 >> 8));
    lcd_io_write_data((uint8_t)(y1 & 0xFF));

    lcd_io_write_cmd(ILI9486_CMD_RAMWR);
}

void ili9486_fill_screen(uint16_t color)
{
    static uint8_t line[ILI9486_WIDTH * LCD_BYTES_PER_PIXEL];

    for (uint32_t i = 0; i < ILI9486_WIDTH; i++)
    {
        line[2 * i] = (uint8_t)(color >> 8);
        line[2 * i + 1] = (uint8_t)(color & 0xFF);
    }

    ili9486_set_window(0, 0, ILI9486_WIDTH - 1, ILI9486_HEIGHT - 1);
    for (uint32_t y = 0; y < ILI9486_HEIGHT; y++)
    {
        lcd_io_write_pixels(line, sizeof(line));
    }
}
