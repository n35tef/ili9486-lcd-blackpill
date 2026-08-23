# ili9486-lcd-blackpill

Bare-metal STM32F411 (BlackPill) firmware driving a 3.5" ILI9486 SPI LCD panel
(the Waveshare 3.5" "C" display) over hardware SPI2 + DMA.

> This README is a living document and will be updated as the project progresses.

## Hardware

- MCU: STM32F411CEUx (BlackPill board), clocked at 100MHz (max for this part) via the
  internal HSI oscillator + PLL, `AHB=100MHz`, `APB1=50MHz`, `APB2=100MHz`.
- Display: 3.5" 480x320 SPI LCD, genuine **ILI9486** controller (see [Findings](#findings-from-bring-up--debugging) below).
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
Core/                   CubeMX-generated startup/HAL glue (clock config, GPIO/SPI/DMA init)
Drivers/ili9486/        Our LCD driver (controller command sequence + low-level SPI IO)
Drivers/CMSIS/          ARM CMSIS core headers (vendor)
Drivers/STM32F4xx_HAL_Driver/  ST HAL library (vendor)
cmake/                  Toolchain file + CubeMX-generated CMake glue
docs/                   Datasheet, debug video, reference blobs (Waveshare .dtbo/.zip)
tools/logic-analysis/   Logic analyzer captures + Python scripts used to reverse-engineer the panel
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

### Findings from bring-up & debugging

The panel initially produced a blank/white screen with no visible activity, despite the SPI bus
looking correct on a logic analyzer and commands matching known ILI9486 init sequences from
several reference drivers.

**What was missing:** the display is sold as a Waveshare 3.5" "C" panel, whose Linux `fbtft`
driver configures the SPI transport with `regwidth=16`. That means every command byte and every
parameter byte is framed as a **16-bit big-endian value** on the wire (`0x00` high byte + the real
byte), not as a raw 8-bit byte per SPI transfer like most ILI9486 reference code assumes. Sending
plain 8-bit command/parameter bytes meant the controller was misinterpreting most of the init
sequence — it looked alive (backlight/power fine) but never actually initialized correctly, hence
a blank panel with normal-looking SPI traffic.

**How it was found:** by decoding the vendor's own `waveshare35c.dtbo` device-tree blob (see
[docs/reference](docs/reference)) and comparing its `regwidth` property against captured SPI
traffic from a known-working reference (Raspberry Pi + `fbtft`) using the scripts in
[tools/logic-analysis](tools/logic-analysis).

**The fix:** `LCD_REGWIDTH16` in [`lcd_io_spi2.h`](Drivers/ili9486/lcd_io_spi2.h) prefixes every
command/data byte with a `0x00` high byte before transmitting (`lcd_io_write_cmd`,
`lcd_io_write_data`, `lcd_io_write_data_buf`). Raw pixel data (`lcd_io_write_pixels`) is **not**
prefixed — pixel writes stay as a plain byte stream, matching `fbtft`'s `write_vmem16_bus8`. This
was the root-cause fix that got the display working.

Secondary findings/fixes made after the panel came alive:
- **Orientation**: the init sequence sets `MADCTL=0x28` (`MV=1`, row/column exchange = landscape).
  `ILI9486_WIDTH`/`ILI9486_HEIGHT` must be `480x320` (landscape) to match, not the controller's
  native `320x480` portrait GRAM — otherwise `CASET`/`PASET` address the wrong axes.
- **Speed**: SPI2 runs at `SPI_BAUDRATEPRESCALER_2` (25MHz, `PCLK1/2`), the fastest this bus
  supports — exceeds the ILI9486 datasheet's ~15MHz write spec but has been reliable in practice.
  Drop to `SPI_BAUDRATEPRESCALER_4` (12.5MHz) if the panel ever shows tearing/glitches.
- **DMA**: `lcd_io_write_pixels()` uses `HAL_SPI_Transmit_DMA()` (DMA1 Stream4, wired up in
  `HAL_SPI_MspInit()`) instead of blocking `HAL_SPI_Transmit()`, since pixel fills move far more
  data than command/parameter writes.
- Only genuine ILI9486 support remains in the driver — earlier experimental code paths for
  ILI9488/ST7796/ILI9341 (tried before the controller was confirmed) have been removed.
