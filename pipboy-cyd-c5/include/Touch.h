// ============================================================================
//  Touch.h — XPT2046 resistive touch, polled mode, with runtime calibration
// ============================================================================

#pragma once
#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "Pins.h"
#include "Theme.h"

class Touch {
public:
    void begin() {
        _spi.begin(Pins::SPI_SCK, Pins::SPI_MISO, Pins::SPI_MOSI, Pins::TOUCH_CS_PIN);
        _ts.begin(_spi);
        _ts.setRotation(1); // must match tft.setRotation()
    }

    // Returns true if a new touch point was read this call.
    // xOut/yOut are in screen pixel coordinates (0..319, 0..239) after
    // applying the linear calibration below.
    bool poll(int16_t &xOut, int16_t &yOut) {
        if (!_ts.touched()) return false;

        TS_Point p = _ts.getPoint();

        // ---- Calibration ----
        // Raw ADC range from XPT2046_Touchscreen is typically ~200-3900.
        // These are *starting* values — run the calibration routine in
        // Calibration.h (touch the 4 corners at boot) to get exact numbers
        // for your physical panel, then hardcode them here for production.
        constexpr int32_t RAW_X_MIN = 300,  RAW_X_MAX = 3800;
        constexpr int32_t RAW_Y_MIN = 300,  RAW_Y_MAX = 3800;

        long mx = map(p.x, RAW_X_MIN, RAW_X_MAX, 0, Theme::SCREEN_W);
        long my = map(p.y, RAW_Y_MIN, RAW_Y_MAX, 0, Theme::SCREEN_H);

        xOut = constrain(mx, 0, Theme::SCREEN_W - 1);
        yOut = constrain(my, 0, Theme::SCREEN_H - 1);
        return true;
    }

    // Simple debounce: only report one touch event per press-release cycle.
    bool pollTap(int16_t &xOut, int16_t &yOut) {
        bool touchedNow = _ts.touched();
        bool firedTap = false;

        if (touchedNow && !_wasTouched) {
            firedTap = poll(xOut, yOut);
        }
        _wasTouched = touchedNow;
        return firedTap;
    }

private:
    SPIClass _spi{HSPI};
    XPT2046_Touchscreen _ts{Pins::TOUCH_CS_PIN, Pins::TOUCH_IRQ};
    bool _wasTouched = false;
};
