#ifndef LVGL_PORT_H
#define LVGL_PORT_H

#include "lvgl.h"

void lvgl_port_init(void);
lv_display_t* lvgl_port_get_display(void);
uint32_t lvgl_port_get_frame_count(void);

#endif /* LVGL_PORT_H */
