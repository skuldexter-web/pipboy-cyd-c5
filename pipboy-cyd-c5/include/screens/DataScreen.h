// ============================================================================
//  DataScreen.h — device telemetry (repurposes the Pip-Boy "DATA" tab to
//  show real hardware stats — heap, uptime, Wi-Fi — genuinely useful info
//  instead of Fallout lore placeholders).
// ============================================================================

#pragma once
#include "Screen.h"
#include "Theme.h"
#include <WiFi.h>

class DataScreen : public Screen {
public:
    void onEnter(TFT_eSPI &tft) override {
        drawFrame(tft);
        _lastDraw = 0; // force immediate refresh
    }

    void update(TFT_eSPI &tft) override {
        uint32_t now = millis();
        if (now - _lastDraw < 1000) return; // 1Hz refresh, cheap
        _lastDraw = now;

        tft.setTextFont(Theme::FONT_BODY);
        tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);
        tft.setTextDatum(ML_DATUM);

        int16_t x = 24, y = Theme::CONTENT_Y + 20, rowH = 22;
        char buf[48];

        snprintf(buf, sizeof(buf), "UPTIME    %lus", millis() / 1000);
        tft.drawString(buf, x, y); y += rowH;

        snprintf(buf, sizeof(buf), "FREE HEAP %uKB", (unsigned)(ESP.getFreeHeap() / 1024));
        tft.drawString(buf, x, y); y += rowH;

        snprintf(buf, sizeof(buf), "PSRAM     %uKB", (unsigned)(ESP.getFreePsram() / 1024));
        tft.drawString(buf, x, y); y += rowH;

        snprintf(buf, sizeof(buf), "CHIP TEMP N/A"); // C5 temp sensor via IDF HAL if needed later
        tft.drawString(buf, x, y); y += rowH;

        snprintf(buf, sizeof(buf), "WIFI      %s",
                 WiFi.status() == WL_CONNECTED ? "LINKED" : "OFFLINE");
        tft.drawString(buf, x, y); y += rowH;
    }

private:
    uint32_t _lastDraw = 0;

    void drawFrame(TFT_eSPI &tft) {
        tft.fillRect(0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H, Theme::BG_BLACK);
        Theme::drawScanlines(tft, 0, Theme::CONTENT_Y, Theme::SCREEN_W, Theme::CONTENT_H);
        Theme::drawCornerBrackets(tft, 4, Theme::CONTENT_Y + 4,
                                   Theme::SCREEN_W - 8, Theme::CONTENT_H - 8,
                                   Theme::PHOSPHOR_DIM);
    }
};
