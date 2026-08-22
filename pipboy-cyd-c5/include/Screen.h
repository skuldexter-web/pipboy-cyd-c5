// ============================================================================
//  Screen.h — interface every menu screen implements
// ============================================================================

#pragma once
#include <TFT_eSPI.h>

class Screen {
public:
    virtual ~Screen() = default;

    // Called once when the screen becomes active — do full-screen redraw here.
    virtual void onEnter(TFT_eSPI &tft) = 0;

    // Called every loop while active — keep this cheap, redraw only deltas.
    virtual void update(TFT_eSPI &tft) {}

    // Called on a tap within the content area (already excludes tab/status bars).
    // x,y are in full-screen coordinates.
    virtual void onTouch(TFT_eSPI &tft, int16_t x, int16_t y) {}
};
