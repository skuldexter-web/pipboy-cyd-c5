// ============================================================================
//  StatScreen.h — S.P.E.C.I.A.L. attribute readout
// ============================================================================

#pragma once
#include "Screen.h"
#include "Theme.h"

class StatScreen : public Screen {
public:
    struct Special {
        const char* label;
        uint8_t value;
    };

    void onEnter(TFT_eSPI &tft) override {
        tft.fillRect(0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H, Theme::BG_BLACK);
        Theme::drawScanlines(tft, 0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H);
        Theme::drawCornerBrackets(tft, 4, Theme::CONTENT_Y + 4,
                                   Theme::SCREEN_W - 8, Theme::CONTENT_H - 8,
                                   Theme::PHOSPHOR_DIM);

        static const Special stats[] = {
            {"STRENGTH",     5},
            {"PERCEPTION",   6},
            {"ENDURANCE",    5},
            {"CHARISMA",     4},
            {"INTELLIGENCE", 7},
            {"AGILITY",      6},
            {"LUCK",         3},
        };

        tft.setTextFont(Theme::FONT_BODY);
        tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);

        int16_t rowY = Theme::CONTENT_Y + 16;
        int16_t rowH = (Theme::CONTENT_H - 32) / 7;

        for (auto &s : stats) {
            tft.setTextDatum(ML_DATUM);
            tft.drawString(s.label, 20, rowY);

            tft.setTextDatum(MR_DATUM);
            char buf[4];
            snprintf(buf, sizeof(buf), "%d", s.value);
            tft.drawString(buf, Theme::SCREEN_W - 24, rowY);

            // mini bar-graph, 10 segments
            int16_t barX = 150, barW = 110, segW = barW / 10;
            for (int i = 0; i < 10; i++) {
                uint16_t c = (i < s.value) ? Theme::PHOSPHOR : Theme::PHOSPHOR_FAINT;
                tft.fillRect(barX + i * segW, rowY - 4, segW - 2, 8, c);
            }
            rowY += rowH;
        }
    }
};
