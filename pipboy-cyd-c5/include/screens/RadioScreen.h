// ============================================================================
//  RadioScreen.h — station list + tuning readout
// ============================================================================

#pragma once
#include "Screen.h"
#include "Theme.h"

class RadioScreen : public Screen {
public:
    void onEnter(TFT_eSPI &tft) override {
        tft.fillRect(0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H, Theme::BG_BLACK);
        Theme::drawScanlines(tft, 0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H);
        Theme::drawCornerBrackets(tft, 4, Theme::CONTENT_Y + 4,
                                   Theme::SCREEN_W - 8, Theme::CONTENT_H - 8,
                                   Theme::PHOSPHOR_DIM);

        static const char* stations[] = {
            "DIAMOND CITY RADIO",
            "CLASSICAL RADIO",
            "MINUTEMEN FREQ",
        };

        tft.setTextFont(Theme::FONT_BODY);
        tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);
        tft.setTextDatum(ML_DATUM);

        int16_t y = Theme::CONTENT_Y + 16;
        for (auto s : stations) {
            tft.drawString(s, 22, y);
            y += 24;
        }
        _touchedRow = -1;
    }

    void onTouch(TFT_eSPI &tft, int16_t x, int16_t y) override {
        int8_t row = (y - (Theme::CONTENT_Y + 4)) / 24;
        if (row < 0 || row > 2) return;
        if (row == _touchedRow) return;
        _touchedRow = row;

        // highlight selected row
        tft.fillRect(0, Theme::CONTENT_Y + 4 + row * 24, Theme::SCREEN_W,
                     20, row == _touchedRow ? Theme::PHOSPHOR_FAINT : Theme::BG_BLACK);
        onEnter(tft); // simplest correct redraw; cheap enough at this res/rate
    }

private:
    int8_t _touchedRow = -1;
};
