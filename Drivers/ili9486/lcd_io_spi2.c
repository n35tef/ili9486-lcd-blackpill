#include "Drivers/ili9486/lcd_io_spi2.h"
#include "main.h"

extern SPI_HandleTypeDef hspi2;

static inline void lcd_cs_low(void)
{
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET);
}
static inline void lcd_cs_high(void)
{
    HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET);
}
static inline void lcd_dc_cmd(void)
{
    HAL_GPIO_WritePin(LCD_DATA_CTL_GPIO_Port, LCD_DATA_CTL_Pin, GPIO_PIN_RESET);
}
static inline void lcd_dc_data(void)
{
    HAL_GPIO_WritePin(LCD_DATA_CTL_GPIO_Port, LCD_DATA_CTL_Pin, GPIO_PIN_SET);
}

static inline void lcd_io_begin_transfer(void) { lcd_cs_low(); }

static inline void lcd_io_end_transfer(void) { lcd_cs_high(); }

void lcd_io_reset(void)
{
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(150);
}

void lcd_io_write_cmd(uint8_t cmd)
{
    lcd_dc_cmd();
    lcd_io_begin_transfer();
#if LCD_REGWIDTH16
    uint8_t tx[2] = {0x00, cmd};
    HAL_SPI_Transmit(&hspi2, tx, 2, HAL_MAX_DELAY);
#else
    HAL_SPI_Transmit(&hspi2, &cmd, 1, HAL_MAX_DELAY);
#endif
    lcd_io_end_transfer();
}

void lcd_io_write_data(uint8_t data)
{
    lcd_dc_data();
    lcd_io_begin_transfer();
#if LCD_REGWIDTH16
    uint8_t tx[2] = {0x00, data};
    HAL_SPI_Transmit(&hspi2, tx, 2, HAL_MAX_DELAY);
#else
    HAL_SPI_Transmit(&hspi2, &data, 1, HAL_MAX_DELAY);
#endif
    lcd_io_end_transfer();
}

void lcd_io_write_data_buf(const uint8_t* buf, size_t len)
{
    lcd_dc_data();
    lcd_io_begin_transfer();
#if LCD_REGWIDTH16
    for (size_t i = 0; i < len; i++)
    {
        uint8_t tx[2] = {0x00, buf[i]};
        HAL_SPI_Transmit(&hspi2, tx, 2, HAL_MAX_DELAY);
    }
#else
    while (len > 0)
    {
        uint16_t chunk = (len > UINT16_MAX) ? UINT16_MAX : (uint16_t)len;
        HAL_SPI_Transmit(&hspi2, (uint8_t*)buf, chunk, HAL_MAX_DELAY);
        buf += chunk;
        len -= chunk;
    }
#endif
    lcd_io_end_transfer();
}

void lcd_io_write_pixels(const uint8_t* buf, size_t len)
{
    lcd_dc_data();
    lcd_io_begin_transfer();
    while (len > 0)
    {
        uint16_t chunk = (len > UINT16_MAX) ? UINT16_MAX : (uint16_t)len;
        HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)buf, chunk);
        while (hspi2.State != HAL_SPI_STATE_READY)
        {
        }
        buf += chunk;
        len -= chunk;
    }
    lcd_io_end_transfer();
}

void lcd_io_write_pixels_async(const uint8_t* buf, size_t len)
{
    lcd_dc_data();
    lcd_io_begin_transfer();
    HAL_SPI_Transmit_DMA(&hspi2, (uint8_t*)buf, (uint16_t)len);
}

void lcd_io_pixels_async_done(void) { lcd_io_end_transfer(); }
