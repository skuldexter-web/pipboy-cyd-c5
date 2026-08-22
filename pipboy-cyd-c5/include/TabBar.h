// ============================================================================
//  TabBar.h — top navigation: STAT / INV / DATA / MAP / RADIO
// ============================================================================

#pragma once
#include <TFT_eSPI.h>
#include "Theme.h"

class TabBar {
public:
    void draw(TFT_eSPI &tft, uint8_t activeIndex) {
        tft.fillRect(0, 0, Theme::SCREEN_W, Theme::TAB_BAR_H, Theme::BG_BLACK);
        tft.drawFastHLine(0, Theme::TAB_BAR_H - 1, Theme::SCREEN_W, Theme::PHOSPHOR_DIM);

        int16_t tabW = Theme::SCREEN_W / Theme::TAB_COUNT;

        tft.setTextDatum(MC_DATUM);
        tft.setFreeFont(nullptr);
        tft.setTextFont(Theme::FONT_BODY);

        for (uint8_t i = 0; i < Theme::TAB_COUNT; i++) {
            int16_t cx = i * tabW + tabW / 2;
            int16_t cy = Theme::TAB_BAR_H / 2;

            bool active = (i == activeIndex);
            uint16_t color = active ? Theme::PHOSPHOR : Theme::PHOSPHOR_DIM;

            if (active) {
                // Bracket the active tab, Pip-Boy style: ⌐STAT¬
                tft.drawFastHLine(i * tabW + 6, 3, tabW - 12, color);
                tft.drawFastVLine(i * tabW + 6, 3, 6, color);
                tft.drawFastVLine(i * tabW + tabW - 6, 3, 6, color);
            }

            tft.setTextColor(color, Theme::BG_BLACK);
            tft.drawString(Theme::TAB_LABELS[i], cx, cy + 4);
        }
    }

    // Maps a touch point to a tab index, or -1 if outside the tab bar.
    int8_t hitTest(int16_t x, int16_t y) {
        if (y < 0 || y >= Theme::TAB_BAR_H) return -1;
        int16_t tabW = Theme::SCREEN_W / Theme::TAB_COUNT;
        int8_t idx = x / tabW;
        if (idx < 0 || idx >= Theme::TAB_COUNT) return -1;
        return idx;
    }
};
