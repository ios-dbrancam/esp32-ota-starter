#pragma once
#include <Arduino.h>
#include "button/button.h"
#include "config/pwm_config.h"

class DeskLight {
    public:
        DeskLight(
            uint8_t pwmPin,
            uint8_t upButtonPin,
            uint8_t downButtonPin,
            const PwmConfig pwmConfig,
            uint16_t debounceTime);

        void initialize();
        void update();

    private:
        uint8_t pwmPin_;
        Button upButton_;
        Button downButton_;
        PwmConfig pwmConfig_;
        uint16_t debounceTime_;
        uint16_t brightnessLevel_;

        void setBrightness();
        void increaseBrightness();
        void decreaseBrightness();
};
