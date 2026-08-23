#include "lvgl_port.h"
#include "Drivers/ili9486/ili9486.h"
#include "Drivers/ili9486/lcd_io_spi2.h"
#include "stm32f4xx_hal.h"

/* 2 × 34 lines × 480 px × 2 B = 65,280 B ≈ 50% of 128 KB RAM */
#define LVGL_BUF_LINES 34
static lv_color_t buf1[LVGL_BUF_LINES * ILI9486_WIDTH];
static lv_color_t buf2[LVGL_BUF_LINES * ILI9486_WIDTH];

static lv_display_t* disp;

static void disp_flush(lv_display_t* d, const lv_area_t* area, uint8_t* px_map)
{
    (void)d;
    uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
    ili9486_set_window((uint16_t)area->x1, (uint16_t)area->y1, (uint16_t)area->x2,
                       (uint16_t)area->y2);
    lcd_io_write_pixels_async(px_map, w * h * LCD_BYTES_PER_PIXEL);
    /* lv_display_flush_ready() is called from HAL_SPI_TxCpltCallback after DMA completes */
}

/* Called by the HAL DMA TX-complete IRQ; releases CS and unblocks LVGL rendering */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    (void)hspi;
    lcd_io_pixels_async_done();
    lv_display_flush_ready(disp);
}

/* 1ms tick fed by SysTick via HAL_SYSTICK_Callback weak override */
void HAL_SYSTICK_Callback(void) { lv_tick_inc(1); }

void lvgl_port_init(void)
{
    lv_init();
    disp = lv_display_create(ILI9486_WIDTH, ILI9486_HEIGHT);
    lv_display_set_flush_cb(disp, disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

lv_display_t* lvgl_port_get_display(void) { return disp; }
