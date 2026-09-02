#include "main.h"
#include "dashboard_ui.h"
#include "lvgl.h"

int main(void)
{
    core_init();
    dashboard_ui_init();

    while (1)
    {
        lv_timer_handler();
    }
}
