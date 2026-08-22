// ============================================================================
//  InvScreen.h — inventory listing
// ============================================================================

#pragma once
#include "Screen.h"
#include "Theme.h"

class InvScreen : public Screen {
public:
    struct Item { const char* name; const char* qty; };

    void onEnter(TFT_eSPI &tft) override {
        tft.fillRect(0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H, Theme::BG_BLACK);
        Theme::drawScanlines(tft, 0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H);
        Theme::drawCornerBrackets(tft, 4, Theme::CONTENT_Y + 4,
                                   Theme::SCREEN_W - 8, Theme::CONTENT_H - 8,
                                   Theme::PHOSPHOR_DIM);

        static const Item items[] = {
            {"10mm Pistol",     "x1"},
            {"Stimpak",         "x0"},
            {"RadAway",         "x0"},
            {"Combat Armor",    "x1"},
            {"Bobby Pin",       "x12"},
            {"Fusion Cell",     "x24"},
        };

        tft.setTextFont(Theme::FONT_BODY);
        tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);

        int16_t rowY = Theme::CONTENT_Y + 14;
        int16_t rowH = 20;

        for (auto &it : items) {
            tft.setTextDatum(ML_DATUM);
            tft.drawString(it.name, 20, rowY);
            tft.setTextDatum(MR_DATUM);
            tft.drawString(it.qty, Theme::SCREEN_W - 24, rowY);
            tft.drawFastHLine(20, rowY + 10, Theme::SCREEN_W - 40, Theme::PHOSPHOR_FAINT);
            rowY += rowH;
        }
    }
};
