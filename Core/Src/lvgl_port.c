#include "lvgl_port.h"
#include "Drivers/ili9486/ili9486.h"
#include "Drivers/ili9486/lcd_io_spi2.h"
#include "stm32f4xx_hal.h"

#define LVGL_BUF_LINES 34
static lv_color_t buf1[LVGL_BUF_LINES * ILI9486_WIDTH];
static lv_color_t buf2[LVGL_BUF_LINES * ILI9486_WIDTH];

static lv_display_t* disp;

static volatile uint32_t s_frame_count;
static volatile bool s_flush_is_last;

static void disp_flush(lv_display_t* d, const lv_area_t* area, uint8_t* px_map)
{
    (void)d;
    uint32_t w = (uint32_t)(area->x2 - area->x1 + 1);
    uint32_t h = (uint32_t)(area->y2 - area->y1 + 1);
    s_flush_is_last = lv_display_flush_is_last(d);
    ili9486_set_window((uint16_t)area->x1, (uint16_t)area->y1, (uint16_t)area->x2,
                       (uint16_t)area->y2);
    lcd_io_write_pixels_async(px_map, w * h * LCD_BYTES_PER_PIXEL);
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    (void)hspi;
    lcd_io_pixels_async_done();
    if (s_flush_is_last)
        s_frame_count++;
    lv_display_flush_ready(disp);
}

void HAL_SYSTICK_Callback(void) { lv_tick_inc(1); }

void lvgl_port_init(void)
{
    lv_init();
    disp = lv_display_create(ILI9486_WIDTH, ILI9486_HEIGHT);
    lv_display_set_flush_cb(disp, disp_flush);
    lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}

lv_display_t* lvgl_port_get_display(void) { return disp; }

uint32_t lvgl_port_get_frame_count(void) { return s_frame_count; }
