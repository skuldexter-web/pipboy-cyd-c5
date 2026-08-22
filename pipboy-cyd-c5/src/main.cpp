// ============================================================================
//  Pip-Boy UI — NM-CYD-C5
//  main.cpp — boot sequence, screen router, touch dispatch loop
// ============================================================================

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Pins.h"
#include "Theme.h"
#include "Touch.h"
#include "TabBar.h"
#include "StatusBar.h"
#include "Screen.h"
#include "screens/StatScreen.h"
#include "screens/InvScreen.h"
#include "screens/DataScreen.h"
#include "screens/MapScreen.h"
#include "screens/RadioScreen.h"

TFT_eSPI tft = TFT_eSPI();
Touch touch;
TabBar tabBar;
StatusBar statusBar;
VitalStats vitals;

StatScreen  stateStat;
InvScreen   stateInv;
DataScreen  stateData;
MapScreen   stateMap;
RadioScreen stateRadio;

Screen* screens[Theme::TAB_COUNT] = {
    &stateStat, &stateInv, &stateData, &stateMap, &stateRadio
};
uint8_t activeTab = 0;

// ---- Boot animation: quick CRT-style power-on flicker + boot text --------
void bootSequence() {
    tft.fillScreen(Theme::BG_BLACK);

    static const char* lines[] = {
        "ROBCO INDUSTRIES (TM) TERMLINK",
        "INITIALIZING...",
        "LOADING PIP-OS v2.1...",
        "CALIBRATING TOUCH MATRIX...",
        "STATUS: NOMINAL",
    };

    tft.setTextFont(Theme::FONT_BODY);
    tft.setTextColor(Theme::PHOSPHOR, Theme::BG_BLACK);
    tft.setTextDatum(TL_DATUM);

    int16_t y = 20;
    for (auto line : lines) {
        tft.drawString(line, 14, y);
        y += 20;
        delay(180);
    }
    delay(300);

    // brief flicker before main UI, sells the CRT-power-on feel
    for (int i = 0; i < 2; i++) {
        tft.fillScreen(Theme::PHOSPHOR_FAINT);
        delay(30);
        tft.fillScreen(Theme::BG_BLACK);
        delay(60);
    }
}

void switchTab(uint8_t idx) {
    if (idx >= Theme::TAB_COUNT) return;
    activeTab = idx;
    tabBar.draw(tft, activeTab);
    statusBar.draw(tft, vitals);
    screens[activeTab]->onEnter(tft);
}

void setup() {
    Serial.begin(115200);

    pinMode(Pins::RGB_LED, OUTPUT);
    digitalWrite(Pins::RGB_LED, LOW);

    tft.init();
    tft.setRotation(1); // landscape, 320x240 — must match Touch::begin()
    tft.fillScreen(Theme::BG_BLACK);

    touch.begin();

    bootSequence();
    switchTab(0);
}

void loop() {
    // ---- Touch dispatch ----
    int16_t tx, ty;
    if (touch.pollTap(tx, ty)) {
        int8_t tab = tabBar.hitTest(tx, ty);
        if (tab >= 0) {
            switchTab((uint8_t)tab);
        } else if (ty < Theme::SCREEN_H - Theme::STATUS_BAR_H) {
            screens[activeTab]->onTouch(tft, tx, ty);
        }
    }

    // ---- Per-frame screen update (animations, live telemetry) ----
    screens[activeTab]->update(tft);

    // Status bar is cheap to redraw at low rate; keeps HP/AP live if the
    // app later wires vitals to real sensors/game state.
    static uint32_t lastStatusDraw = 0;
    if (millis() - lastStatusDraw > 1000) {
        lastStatusDraw = millis();
        statusBar.draw(tft, vitals);
    }
}
