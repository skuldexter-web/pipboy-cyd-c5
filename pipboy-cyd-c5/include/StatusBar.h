// ============================================================================
//  StatusBar.h — bottom bar: HP / LEVEL / AP, mirrors classic Pip-Boy footer
// ============================================================================

#pragma once
#include <TFT_eSPI.h>
#include "Theme.h"

struct VitalStats {
    int hp = 100, hpMax = 100;
    int ap = 90,  apMax = 90;
    int level = 1;
};

class StatusBar {
public:
    void draw(TFT_eSPI &tft, const VitalStats &v) {
        int16_t y = Theme::SCREEN_H - Theme::STATUS_BAR_H;
        tft.fillRect(0, y, Theme::SCREEN_W, Theme::STATUS_BAR_H, Theme::BG_BLACK);
        tft.drawFastHLine(0, y, Theme::SCREEN_W, Theme::PHOSPHOR_DIM);

        tft.setTextFont(Theme::FONT_SMALL);
        tft.setTextSize(1);
        tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);
        tft.setTextDatum(ML_DATUM);

        char buf[24];
        snprintf(buf, sizeof(buf), "HP %d/%d", v.hp, v.hpMax);
        tft.drawString(buf, 4, y + Theme::STATUS_BAR_H / 2);

        snprintf(buf, sizeof(buf), "LVL %d", v.level);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(buf, Theme::SCREEN_W / 2, y + Theme::STATUS_BAR_H / 2);

        snprintf(buf, sizeof(buf), "AP %d/%d", v.ap, v.apMax);
        tft.setTextDatum(MR_DATUM);
        tft.drawString(buf, Theme::SCREEN_W - 4, y + Theme::STATUS_BAR_H / 2);
    }
};
