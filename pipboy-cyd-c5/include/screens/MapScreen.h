// ============================================================================
//  MapScreen.h — radar-sweep placeholder (swap for real GPS/tile map later)
// ============================================================================

#pragma once
#include "Screen.h"
#include "Theme.h"

class MapScreen : public Screen {
public:
    void onEnter(TFT_eSPI &tft) override {
        tft.fillRect(0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H, Theme::BG_BLACK);
        _cx = Theme::SCREEN_W / 2;
        _cy = Theme::CONTENT_Y + Theme::CONTENT_H / 2;
        _r  = min(Theme::CONTENT_H, Theme::SCREEN_W) / 2 - 12;

        tft.drawCircle(_cx, _cy, _r, Theme::PHOSPHOR_DIM);
        tft.drawCircle(_cx, _cy, _r * 2 / 3, Theme::PHOSPHOR_FAINT);
        tft.drawCircle(_cx, _cy, _r / 3, Theme::PHOSPHOR_FAINT);
        tft.drawFastHLine(_cx - _r, _cy, _r * 2, Theme::PHOSPHOR_FAINT);
        tft.drawFastVLine(_cx, _cy - _r, _r * 2, Theme::PHOSPHOR_FAINT);

        _angle = 0;
        _lastFrame = 0;
    }

    void update(TFT_eSPI &tft) override {
        uint32_t now = millis();
        if (now - _lastFrame < 40) return; // ~25fps sweep
        _lastFrame = now;

        // erase previous sweep line by redrawing it in faint color
        float prevRad = radians(_angle);
        tft.drawLine(_cx, _cy,
                     _cx + _r * cosf(prevRad), _cy + _r * sinf(prevRad),
                     Theme::BG_BLACK);
        // redraw ring segments the erase may have clipped
        tft.drawCircle(_cx, _cy, _r, Theme::PHOSPHOR_DIM);

        _angle = (_angle + 4) % 360;
        float rad = radians(_angle);
        tft.drawLine(_cx, _cy,
                     _cx + _r * cosf(rad), _cy + _r * sinf(rad),
                     Theme::PHOSPHOR);
    }

private:
    int16_t _cx, _cy, _r;
    int _angle = 0;
    uint32_t _lastFrame = 0;
};
