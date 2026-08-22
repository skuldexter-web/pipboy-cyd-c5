// ============================================================================
//  Theme.h — RobCo / Vault-Tec visual language
//  Monochrome phosphor palette, layout geometry, tab bar definitions.
// ============================================================================

#pragma once
#include <TFT_eSPI.h>

namespace Theme {

    // ---- Phosphor color selection -----------------------------------------
    // Swap PHOSPHOR_MODE to change the whole UI's tint at compile time.
    // 0 = classic CRT green, 1 = amber, 2 = blue
    #define PHOSPHOR_MODE 0

    #if PHOSPHOR_MODE == 0
        constexpr uint16_t PHOSPHOR      = 0x07EC; // ~#00FF66 in RGB565
        constexpr uint16_t PHOSPHOR_DIM  = 0x03E8; // dimmed variant for inactive UI
        constexpr uint16_t PHOSPHOR_FAINT= 0x0200; // scanline / grid tint
    #elif PHOSPHOR_MODE == 1
        constexpr uint16_t PHOSPHOR      = 0xFD00; // amber
        constexpr uint16_t PHOSPHOR_DIM  = 0x7A80;
        constexpr uint16_t PHOSPHOR_FAINT= 0x3000;
    #else
        constexpr uint16_t PHOSPHOR      = 0x04FF; // blue
        constexpr uint16_t PHOSPHOR_DIM  = 0x0299;
        constexpr uint16_t PHOSPHOR_FAINT= 0x0011;
    #endif

    constexpr uint16_t BG_BLACK   = 0x0000;
    constexpr uint16_t ALERT_RED  = 0xF800;

    // ---- Screen geometry (landscape, rotation = 1) ----
    constexpr int16_t SCREEN_W = 320;
    constexpr int16_t SCREEN_H = 240;

    constexpr int16_t TAB_BAR_H   = 26;
    constexpr int16_t STATUS_BAR_H = 18;
    constexpr int16_t CONTENT_Y   = TAB_BAR_H;
    constexpr int16_t CONTENT_H   = SCREEN_H - TAB_BAR_H - STATUS_BAR_H;

    constexpr int16_t CORNER_LEN  = 10; // corner-bracket arm length

    // ---- Fonts (TFT_eSPI GFXFF free fonts loaded via LOAD_GFXFF) ----
    // Using built-in fixed-width font (Font 2 / GLCD) keeps things fast and
    // dependency-free; swap for a custom Space-Grotesk/JetBrains-Mono .vlw
    // smooth font later by dropping it in /data and calling loadFont().
    constexpr uint8_t FONT_SMALL = 1;   // GLCD 6x8, scaled
    constexpr uint8_t FONT_BODY  = 2;   // built-in 16px font
    constexpr uint8_t FONT_LARGE = 4;   // built-in 26px font

    // ---- Tab bar labels — mirrors Pip-Boy top nav ----
    static const char* const TAB_LABELS[] = { "STAT", "INV", "DATA", "MAP", "RADIO" };
    constexpr uint8_t TAB_COUNT = 5;

    // Draws a corner-bracket frame like Pip-Boy panel edges: [___    ___]
    inline void drawCornerBrackets(TFT_eSPI &tft, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
        // top-left
        tft.drawFastHLine(x, y, CORNER_LEN, color);
        tft.drawFastVLine(x, y, CORNER_LEN, color);
        // top-right
        tft.drawFastHLine(x + w - CORNER_LEN, y, CORNER_LEN, color);
        tft.drawFastVLine(x + w - 1, y, CORNER_LEN, color);
        // bottom-left
        tft.drawFastHLine(x, y + h - 1, CORNER_LEN, color);
        tft.drawFastVLine(x, y + h - CORNER_LEN, CORNER_LEN, color);
        // bottom-right
        tft.drawFastHLine(x + w - CORNER_LEN, y + h - 1, CORNER_LEN, color);
        tft.drawFastVLine(x + w - 1, y + h - CORNER_LEN, CORNER_LEN, color);
    }

    // Cheap scanline overlay: draws faint horizontal lines every 3rd row
    // over a given region. Call sparingly (it's a full-width loop) — best
    // used once per screen redraw, not per frame in a tight animation loop.
    inline void drawScanlines(TFT_eSPI &tft, int16_t x, int16_t y, int16_t w, int16_t h) {
        for (int16_t row = y; row < y + h; row += 3) {
            tft.drawFastHLine(x, row, w, PHOSPHOR_FAINT);
        }
    }

} // namespace Theme
