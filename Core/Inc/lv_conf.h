/* clang-format off */
#if 1

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/* Color depth: 16 = RGB565 (matches ILI9486 pixel format) */
#define LV_COLOR_DEPTH 16

/* Use LVGL's built-in malloc pool; no OS/heap dependency */
#define LV_USE_STDLIB_MALLOC    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_STRING    LV_STDLIB_BUILTIN
#define LV_USE_STDLIB_SPRINTF   LV_STDLIB_BUILTIN

/* 16KB internal heap for widget/style/animation objects */
#define LV_MEM_SIZE (16 * 1024U)

/* ~60fps target (16ms) */
#define LV_DEF_REFR_PERIOD 16

/* DPI for a 3.5" 480×320 panel */
#define LV_DPI_DEF 160

/* No OS */
#define LV_USE_OS LV_OS_NONE

/* SW renderer only; disable all GPU/accelerator backends */
#define LV_USE_DRAW_SW 1
#define LV_USE_NEMA_GFX 0
#define LV_USE_DRAW_VGLITE 0
#define LV_USE_PXP 0
#define LV_USE_DRAW_G2D 0
#define LV_USE_DRAW_DAVE2D 0
#define LV_USE_DRAW_SDL 0
#define LV_USE_DRAW_VG_LITE 0
#define LV_USE_DRAW_DMA2D 0
#define LV_USE_DRAW_OPENGLES 0

/* Keep only RGB565 color format support in the SW renderer */
#define LV_DRAW_SW_SUPPORT_RGB565       1
#define LV_DRAW_SW_SUPPORT_RGB565_SWAPPED 0
#define LV_DRAW_SW_SUPPORT_RGB565A8     0
#define LV_DRAW_SW_SUPPORT_RGB888       0
#define LV_DRAW_SW_SUPPORT_XRGB8888     0
#define LV_DRAW_SW_SUPPORT_ARGB8888     0
#define LV_DRAW_SW_SUPPORT_ARGB8888_PREMULTIPLIED 0
#define LV_DRAW_SW_SUPPORT_L8           0
#define LV_DRAW_SW_SUPPORT_AL88         0
#define LV_DRAW_SW_SUPPORT_A8           0
#define LV_DRAW_SW_SUPPORT_I1           0

/* Single SW draw unit; no multi-threading */
#define LV_DRAW_SW_DRAW_UNIT_CNT 1
#define LV_USE_DRAW_ARM2D_SYNC   0
#define LV_USE_NATIVE_HELIUM_ASM 0

/* Complex drawing (gradients, shadows) enabled; cache sizes tuned for 16KB heap */
#define LV_DRAW_SW_COMPLEX         1
#define LV_DRAW_SW_SHADOW_CACHE_SIZE 0
#define LV_DRAW_SW_CIRCLE_CACHE_SIZE 4

/* 24KB layer buffer for intermediate blending; fits in remaining RAM */
#define LV_DRAW_LAYER_SIMPLE_BUF_SIZE (24 * 1024)

/* Logging: off for release builds */
#define LV_USE_LOG 0

/* Fast asserts only (null/malloc checks); no expensive integrity scans */
#define LV_USE_ASSERT_NULL          1
#define LV_USE_ASSERT_MALLOC        1
#define LV_USE_ASSERT_STYLE         0
#define LV_USE_ASSERT_MEM_INTEGRITY 0
#define LV_USE_ASSERT_OBJ           0

/* Default font: Montserrat 14 only; all other sizes off */
#define LV_FONT_MONTSERRAT_8  0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 0
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 0
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 0
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0
#define LV_FONT_SIMSUN_14_CJK            0
#define LV_FONT_SIMSUN_16_CJK            0
#define LV_FONT_UNSCII_8  0
#define LV_FONT_UNSCII_16 0

/* No filesystem drivers */
#define LV_USE_FS_STDIO   0
#define LV_USE_FS_POSIX   0
#define LV_USE_FS_WIN32   0
#define LV_USE_FS_FATFS   0
#define LV_USE_FS_MEMFS   0
#define LV_USE_FS_LITTLEFS 0

/* No image decoders */
#define LV_USE_LIBPNG         0
#define LV_USE_BMP            0
#define LV_USE_LIBJPEG_TURBO  0
#define LV_USE_GIF            0
#define LV_USE_QRCODE         0
#define LV_USE_BARCODE        0
#define LV_USE_FREETYPE       0
#define LV_USE_TINY_TTF       0
#define LV_USE_LOTTIE         0
#define LV_USE_RLOTTIE        0

/* No diagnostics/profiling */
#define LV_USE_SYSMON   0
#define LV_USE_PROFILER 0
#define LV_USE_PERF_MONITOR 0
#define LV_USE_MEM_MONITOR  0

/* No demos or examples */
#define LV_USE_DEMO_WIDGETS          0
#define LV_USE_DEMO_BENCHMARK        0
#define LV_USE_DEMO_RENDER           0
#define LV_USE_DEMO_STRESS           0
#define LV_USE_DEMO_MUSIC            0
#define LV_USE_DEMO_FLEX_LAYOUT      0
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0

/* Standard widgets: keep all (disable individually later as needed) */
#define LV_USE_ARC        1
#define LV_USE_BAR        1
#define LV_USE_BUTTON     1
#define LV_USE_BUTTONMATRIX 1
#define LV_USE_CALENDAR   1
#define LV_USE_CANVAS     1
#define LV_USE_CHART      1
#define LV_USE_CHECKBOX   1
#define LV_USE_DROPDOWN   1
#define LV_USE_IMAGE      1
#define LV_USE_IMAGEBUTTON 1
#define LV_USE_KEYBOARD   1
#define LV_USE_LABEL      1
#define LV_USE_LED        1
#define LV_USE_LINE       1
#define LV_USE_LIST       1
#define LV_USE_MENU       1
#define LV_USE_MSGBOX     1
#define LV_USE_ROLLER     1
#define LV_USE_SCALE      1
#define LV_USE_SLIDER     1
#define LV_USE_SPAN       1
#define LV_USE_SPINBOX    1
#define LV_USE_SPINNER    1
#define LV_USE_SWITCH     1
#define LV_USE_TABLE      1
#define LV_USE_TABVIEW    1
#define LV_USE_TEXTAREA   1
#define LV_USE_TILEVIEW   1
#define LV_USE_WIN        1

/* Layouts */
#define LV_USE_FLEX 1
#define LV_USE_GRID 1

#endif /* LV_CONF_H */
#endif /* end enable content */
