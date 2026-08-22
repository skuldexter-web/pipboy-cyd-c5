// ============================================================================
//  Pins.h — NM-CYD-C5 hardware pin map (RockBase IoT official pinout)
//  Source: https://wiki.rockbaseiot.com/docs/products/nm-cyd-c5/
//
//  TFT_eSPI's own pins are set via build_flags in platformio.ini (that's
//  where the library actually reads them from). This file is for pins used
//  directly by *our* application code — touch IRQ, RGB LED, SD, expansion.
//
//  ⚠ VERIFY BEFORE FIRST POWER-UP:
//  The official pinout table publishes SCK/MISO/MOSI/CS for the shared SPI
//  bus, but does not publish TFT_DC or TFT_RST/backlight in the page I could
//  retrieve. Those are set as placeholders below (matching the classic
//  ILI9341 CYD convention: DC=GPIO4 shared header, RST tied to EN, backlight
//  always-on/PWM). Cross-check against the schematic in the NM-CYD-C5 GitHub
//  repo (Documentation/2-sch) before flashing, and correct here + in
//  platformio.ini's TFT_DC/TFT_RST flags if they differ. Do NOT probe pins
//  live with the display powered to "find out" — check the schematic first.
// ============================================================================

#pragma once
#include <Arduino.h>

namespace Pins {

    // ---- Shared SPI bus (Display / Touch / SD card) ----
    constexpr uint8_t SPI_SCK   = 6;
    constexpr uint8_t SPI_MISO  = 2;
    constexpr uint8_t SPI_MOSI  = 7;

    constexpr uint8_t TFT_CS_PIN   = 23;
    constexpr uint8_t TOUCH_CS_PIN = 1;
    constexpr uint8_t SD_CS_PIN    = 10;

    // Touch interrupt line — NOT in the official table. XPT2046_Touchscreen
    // works polled without it (used that way in Touch.cpp), but if your unit
    // breaks out a T_IRQ pin, set it here for lower-latency, lower-power reads.
    constexpr int8_t TOUCH_IRQ = -1;   // -1 = polled mode, no IRQ wired

    // ---- I2C expansion connector (CN1) ----
    constexpr uint8_t I2C_SDA = 9;
    constexpr uint8_t I2C_SCL = 8;

    // ---- LP-UART (P5) — reserved for GPS module ----
    constexpr uint8_t GPS_RX = 4;
    constexpr uint8_t GPS_TX = 5;

    // ---- Onboard RGB status LED ----
    constexpr uint8_t RGB_LED = 8;

    // ---- Extension IO (P1) ----
    constexpr uint8_t EXT_IO_1 = 4;
    constexpr uint8_t EXT_IO_2 = 8;
    constexpr uint8_t EXT_IO_3 = 26;

} // namespace Pins
