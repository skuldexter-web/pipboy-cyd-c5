# PIP-OS — Pip-Boy UI for the NM-CYD-C5

A RobCo/Vault-Tec-styled touch UI (STAT / INV / DATA / MAP / RADIO tabs,
green-phosphor CRT look, corner-bracket framing, scanlines) for the
**NM-CYD-C5**: an ESP32-C5-based Cheap Yellow Display clone with a 2.8"
320×240 ST7789 panel and XPT2046 capacitive touch.

> ⚠ **This is bleeding-edge hardware.** The ESP32-C5 (RISC-V, dual-band
> Wi-Fi 6) shipped very recently and mainline toolchain support is still
> stabilizing. This project depends on a community fork of the PlatformIO
> Espressif32 platform, not the stock one. Expect to need patched TFT_eSPI
> processor files (instructions below) pulled from the board vendor's repo.
> Budget an extra hour for toolchain friction beyond a normal ESP32 project.

---

## 1. Hardware reference

| Spec | Value |
|---|---|
| MCU | ESP32-C5-WROOM-1, RISC-V, 240 MHz |
| Flash / PSRAM | 16 MB / 8 MB |
| Display | 2.8" 320×240, **ST7789** driver (not ILI9341 — see note below) |
| Touch | XPT2046, capacitive, shares the display's SPI bus |
| USB | Two USB-C ports: one native ESP32-C5 (programming), one CH340 (UART-only) |

**Display driver correction:** your prompt specified ILI9341. The NM-CYD-C5's
default panel driver is ST7789; the board *can* be built with an ILI9341
variant panel, but ST7789 is the default that ships. This project is
configured for ST7789 (`build_flags` in `platformio.ini`). If your unit uses
an ILI9341 panel, change `ST7789_DRIVER=1` to `ILI9341_DRIVER=1` — the
pin map and rest of the code are unaffected.

### Pinout (official, from RockBase IoT's NM-CYD-C5 wiki)

| Device | SCK | MISO | MOSI | CS |
|---|---|---|---|---|
| Display (ST7789) | GPIO 6 | GPIO 2 | GPIO 7 | GPIO 23 |
| Touch (XPT2046) | GPIO 6 | GPIO 2 | GPIO 7 | GPIO 1 |
| SD Card | GPIO 6 | GPIO 2 | GPIO 7 | GPIO 10 |

RGB status LED: GPIO 8. I2C header (CN1): SDA=GPIO9, SCL=GPIO8. LP-UART for
GPS (P5): RX=GPIO4, TX=GPIO5.

> ⚠ **TFT_DC / TFT_RST are not published in the vendor's pinout table** and
> are set as placeholders in `include/Pins.h` and `platformio.ini`
> (`TFT_DC=4`, `TFT_RST=-1`). **Before powering up the display, check the
> schematic** at `github.com/RockBase-iot/NM-CYD-C5/tree/main/Documention/2-sch`
> and correct these two values if they differ — driving the wrong GPIO into
> DC won't damage anything, but the display will show garbage or stay blank
> until it matches your actual board revision.

---

## 2. Project layout

```
pipboy-cyd-c5/
├── platformio.ini            # board/toolchain config, TFT_eSPI build_flags
├── include/
│   ├── Pins.h                 # single source of truth for app-level GPIO
│   ├── Theme.h                # phosphor palette, layout geometry, chrome
│   ├── Touch.h                # XPT2046 wrapper w/ calibration + debounce
│   ├── TabBar.h                # top nav: STAT/INV/DATA/MAP/RADIO
│   ├── StatusBar.h             # bottom HP/LEVEL/AP bar
│   ├── Screen.h                # base interface for all screens
│   └── screens/
│       ├── StatScreen.h        # S.P.E.C.I.A.L. bar-graph readout
│       ├── InvScreen.h         # inventory list
│       ├── DataScreen.h        # live device telemetry (heap/uptime/wifi)
│       ├── MapScreen.h         # animated radar sweep placeholder
│       └── RadioScreen.h       # station list w/ tap-to-select
├── src/
│   └── main.cpp                # boot sequence, screen router, touch dispatch
└── webflasher/
    ├── index.html               # ESP Web Tools browser flasher page
    └── manifest.json            # flash image manifest (chip, offsets)
```

Each screen is a self-contained class implementing `onEnter()` / `update()` /
`onTouch()` — add a new one by copying `InvScreen.h`, adding it to
`Theme::TAB_LABELS`, bumping `TAB_COUNT`, and registering it in the
`screens[]` array in `main.cpp`.

---

## 3. Toolchain setup

### Install PlatformIO

```bash
pip install platformio --break-system-packages
# or, if you prefer the VS Code extension: install "PlatformIO IDE" from the marketplace
```

### Patch TFT_eSPI for ESP32-C5

TFT_eSPI's stock release does not yet include ESP32-C5 processor bindings.
The board vendor ships the missing files in their repo. Do this **once**,
after PlatformIO has fetched the library on first build:

```bash
# 1. First build will fail — that's expected, it fetches TFT_eSPI into .pio/libdeps first
pio run -e nm-cyd-c5 || true

# 2. Pull the vendor's patched processor files
git clone --depth 1 https://github.com/RockBase-iot/NM-CYD-C5.git /tmp/nm-cyd-c5-vendor

# 3. Copy the ESP32-C5 processor binding into TFT_eSPI's Processors folder
LIBDIR=".pio/libdeps/nm-cyd-c5/TFT_eSPI"
cp /tmp/nm-cyd-c5-vendor/Demos/Arduino/libraries/TFT_eSPI/Processors/TFT_eSPI_ESP32_C5.* \
   "$LIBDIR/Processors/"

# 4. Patch TFT_eSPI.h and TFT_eSPI.cpp to recognize CONFIG_IDF_TARGET_ESP32C5
#    — diff the vendor's modified TFT_eSPI.h/.cpp against the stock ones to
#    see the exact #ifdef additions, then apply the same to $LIBDIR/TFT_eSPI.h
#    and $LIBDIR/TFT_eSPI.cpp. This is a small, mechanical patch (a few
#    #elif defined(CONFIG_IDF_TARGET_ESP32C5) blocks mirroring the existing
#    C3/C6 ones) but vendor file layout can shift between repo revisions, so
#    diff rather than blind-copy.
diff /tmp/nm-cyd-c5-vendor/Demos/Arduino/libraries/TFT_eSPI/TFT_eSPI.h "$LIBDIR/TFT_eSPI.h"
diff /tmp/nm-cyd-c5-vendor/Demos/Arduino/libraries/TFT_eSPI/TFT_eSPI.cpp "$LIBDIR/TFT_eSPI.cpp"
```

This step is exactly what the official RockBase wiki instructs ("add the
`TFT_eSPI_ESP32_C5.c/h` files to the Processors folder and update
`TFT_eSPI.c/h` with `CONFIG_IDF_TARGET_ESP32C5`"), automated into copy/diff
commands. If a newer TFT_eSPI release ships native C5 support by the time
you read this, this whole step becomes unnecessary — try a plain
`pio run` first and only patch if it fails with an unrecognized-target error.

---

## 4. Build

```bash
cd pipboy-cyd-c5
pio run -e nm-cyd-c5
```

Output binaries land in `.pio/build/nm-cyd-c5/`:
- `bootloader.bin`
- `partitions.bin`
- `firmware.bin`

## 5. Flash over USB (development)

Connect the **ESP32-C5 programming USB-C port** (not the CH340 one) and run:

```bash
pio run -e nm-cyd-c5 -t upload
pio device monitor -e nm-cyd-c5   # serial console, 115200 baud
```

If upload hangs waiting for the device to reset into download mode, hold the
board's BOOT button, tap RESET, then release BOOT — auto-reset timing on
early ESP32-C5 boards can be inconsistent with some esptool versions.

## 6. Flash via browser (ESP Web Tools)

For distributing prebuilt firmware without requiring PlatformIO on the
receiving end:

```bash
# copy the three build outputs into the webflasher's firmware/ folder
mkdir -p webflasher/firmware
cp .pio/build/nm-cyd-c5/bootloader.bin webflasher/firmware/
cp .pio/build/nm-cyd-c5/partitions.bin webflasher/firmware/
cp .pio/build/nm-cyd-c5/firmware.bin   webflasher/firmware/

# serve over HTTPS or localhost — Web Serial requires a secure context
cd webflasher
python3 -m http.server 8080
# open http://localhost:8080 in Chrome/Edge/Opera (Web Serial is not
# supported in Firefox or Safari)
```

Click **CONNECT & FLASH PIP-OS**, pick the NM-CYD-C5's serial port from the
browser's device picker, and the manifest (`chipFamily: "ESP32-C5"`, correct
0x0/0x8000/0x10000 offsets for a C-series/S3-class chip — note this differs
from classic ESP32, which uses a 0x1000 bootloader offset) handles the rest.

To host this publicly (e.g. GitHub Pages), just push the `webflasher/`
folder as-is — GitHub Pages serves over HTTPS by default, satisfying Web
Serial's secure-context requirement.

---

## 7. Calibrating touch

The XPT2046 raw-to-pixel mapping in `Touch.h` (`RAW_X_MIN/MAX`,
`RAW_Y_MIN/MAX`) uses reasonable defaults but every physical panel varies
slightly. For accurate touch:

1. Add a one-time calibration screen that prompts the user to tap all four
   corners, logs the raw `TS_Point` values over serial.
2. Read those four raw values off the serial monitor.
3. Hardcode them into the `RAW_X_MIN/MAX`/`RAW_Y_MIN/MAX` constants.

This project ships with sane starting defaults so the UI is usable
immediately; tighten them once you have real numbers from your unit.

---

## 8. Customizing the look

- **Color scheme**: flip `PHOSPHOR_MODE` in `Theme.h` between `0` (green),
  `1` (amber), `2` (blue) — every screen inherits the change automatically
  since all drawing goes through `Theme::PHOSPHOR*` constants.
- **Fonts**: currently uses TFT_eSPI's built-in fixed fonts for
  dependency-free reliability. To use a proper monospace retro font (e.g.
  JetBrains Mono), convert a `.ttf` to TFT_eSPI's `.vlw` smooth-font format
  with the [Processing font tool](https://github.com/Bodmer/TFT_eSPI/tree/master/Tools/Create_Smooth_Font),
  drop the `.vlw` in `/data`, flash SPIFFS/LittleFS, and call
  `tft.loadFont("YourFont")` before drawing.
- **New screens**: implement `Screen`, add to `screens[]` in `main.cpp`, add
  a label to `Theme::TAB_LABELS`.

---

## 9. Known limitations / next steps

- `MapScreen` is a radar-sweep placeholder, not a real map — wire it to the
  LP-UART GPS header (P5, RX=GPIO4/TX=GPIO5) and a tile cache on the SD card
  for real functionality.
- `RadioScreen` has no actual audio path — the NM-CYD-C5 pinout referenced
  here doesn't list a dedicated I2S/DAC pin, so audio output needs
  confirming against your board's schematic before wiring anything up.
- Status bar (`HP`/`AP`/`LEVEL`) is static placeholder data (`VitalStats` in
  `main.cpp`) — hook it to real sensors or app state as needed.
- Touch calibration constants are defaults, not calibrated to a specific
  physical unit — see section 7.
