#pragma once
#include <stdint.h>
#include "button/button.h"
#include "config/pwm_config.h"

class DeskLight {
    public:
        DeskLight(
            const char* name,
            uint8_t pwmPin,
            uint8_t upButtonPin,
            uint8_t downButtonPin,
            const PwmConfig pwmConfig,
            unsigned long debounceTime,
            unsigned long longPressTime);

        void initialize();
        void update();

    private:
        const char* name_;
        uint8_t pwmPin_;
        Button upButton_;
        Button downButton_;
        PwmConfig pwmConfig_;
        uint8_t brightnessStep_;
        uint32_t currentPwm_;
        uint32_t initialPwm_;
        uint32_t targetPwm_;
        unsigned long fadeStartTime_;
        unsigned long fadeDuration_;

        void setBrightness();
        void processFade(unsigned long now);
        void increaseBrightness();
        void decreaseBrightness();
        void setMaxBrightness();
        void setMinBrightness();
};
