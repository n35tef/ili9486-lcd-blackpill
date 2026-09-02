# ili9486-lcd-blackpill

Bare-metal STM32F411 (BlackPill) firmware driving a 3.5" ILI9486 SPI LCD panel
(the Waveshare 3.5" "C" display) over hardware SPI2 + DMA.

## Hardware

- MCU: STM32F411CEUx (BlackPill board), clocked at 100MHz (max for this part) via the
  internal HSI oscillator + PLL, `AHB=100MHz`, `APB1=50MHz`, `APB2=100MHz`.
- Display: 3.5" 480x320 SPI LCD, genuine **ILI9486** controller.
- Wiring (see [Core/Inc/main.h](Core/Inc/main.h)):

  | Signal        | Pin        |
  |---------------|------------|
  | SPI2 SCK/MOSI | hardware SPI2 (AF) |
  | `SPI2_CS`     | PB3        |
  | `LCD_DATA_CTL` (D/C) | PB12 |
  | `LCD_RST`     | PB5        |
  | `USER_LED`    | PC13       |
  | `USER_KEY`    | PA0        |

## Project layout

```
Core/                   CubeMX-generated startup/HAL glue (clock config, GPIO/SPI/DMA init) + core_init()
Drivers/ili9486/        LCD driver (controller command sequence + low-level SPI IO)
Drivers/CMSIS/          ARM CMSIS core headers (vendor)
Drivers/STM32F4xx_HAL_Driver/  ST HAL library (vendor)
apps/dashboard/         Demo car-dashboard LVGL app (entry point, UI, fonts)
cmake/                  Toolchain file + CubeMX-generated CMake glue
docs/                   Datasheet and reference blobs (Waveshare .dtbo/.zip)
```

## Building

```sh
cmake --preset Debug
cmake --build build/Debug
```

## Driver

The driver is split in two layers:

- [`Drivers/ili9486/ili9486.c`](Drivers/ili9486/ili9486.c) — controller-level API: init sequence,
  `CASET`/`PASET`/`RAMWR` addressing window, fill-screen helper.
- [`Drivers/ili9486/lcd_io_spi2.c`](Drivers/ili9486/lcd_io_spi2.c) — low-level IO: reset pulse,
  command/data byte writes, and DMA-driven pixel streaming over SPI2.

The display uses `regwidth=16` SPI framing (every command/parameter byte sent as a 16-bit
big-endian value), controlled by `LCD_REGWIDTH16` in
[`lcd_io_spi2.h`](Drivers/ili9486/lcd_io_spi2.h). Raw pixel data (`lcd_io_write_pixels`) is not
prefixed, matching `fbtft`'s `write_vmem16_bus8`.

## Apps

[`Core/Src/main.c`](Core/Src/main.c) exposes `core_init()` (clock/GPIO/DMA/SPI setup + LCD/LVGL
init) for demo apps to call. [`apps/dashboard`](apps/dashboard) is the current demo: it calls
`core_init()`, then `dashboard_ui_init()`, then runs the LVGL timer loop.
