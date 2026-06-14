#pragma once
#include <stdint.h>
#include "button/button.h"
#include "config/pwm_config.h"

class DeskLight {
    public:
        DeskLight(
            uint8_t pwmPin,
            const char* id,
            uint8_t upButtonPin,
            uint8_t downButtonPin,
            const PwmConfig pwmConfig,
            unsigned long debounceTime,
            unsigned long longPressTime,
            uint8_t defaultOnStep = 2);

        void initialize();
        void update();

        bool isOn() const;
        const char* getId() const;
        void setBrightnessStep(int step);
        void turnOn();
        void turnOff();

    private:
        uint8_t pwmPin_;
        const char* id_;
        Button upButton_;
        Button downButton_;
        PwmConfig pwmConfig_;
        uint8_t defaultOnStep_;
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
