#include "desk_light/desk_light.h"
#include "config/pwm_config.h"

DeskLight::DeskLight(uint8_t pwmPin,
                     uint8_t upButtonPin,
                     uint8_t downButtonPin,
                     PwmConfig pwmConfig,
                     unsigned long debounceTime,
                     unsigned long longPressTime)
    : pwmPin_(pwmPin),
      upButton_(upButtonPin, debounceTime, longPressTime),
      downButton_(downButtonPin, debounceTime, longPressTime),
      pwmConfig_(pwmConfig),
      brightnessStep_(0) {}

void DeskLight::initialize() {
    ledcAttach(pwmPin_, pwmConfig_.frequency, pwmConfig_.resolution);
    setBrightness();
    upButton_.initialize();
    downButton_.initialize();
}

void DeskLight::update() {
    unsigned long now = millis();

    upButton_.update(now);
    downButton_.update(now);

    if (upButton_.wasTapped()) {
        increaseBrightness();
    }

    if (downButton_.wasTapped()) {
        decreaseBrightness();
    }

    if (upButton_.wasLongPressed()) {
        setMaxBrightness();
    }

    if (downButton_.wasLongPressed()) {
        setMinBrightness();
    }
}

void DeskLight::setBrightness() {
    // Gamma correction (Logarithmic)
    // OUT = (IN / MaxIN)^Gamma * MaxOUT
    float normalized = (float)brightnessStep_ / (float)pwmConfig_.steps;
    float corrected = pow(normalized, pwmConfig_.gamma);
    uint32_t brightnessLevel = (uint32_t)(corrected * pwmConfig_.upperLimit);
    ledcWrite(pwmPin_, brightnessLevel);
}

void DeskLight::increaseBrightness() {
    brightnessStep_ = min<int>(brightnessStep_ + 1, pwmConfig_.steps);
    setBrightness();
}

void DeskLight::decreaseBrightness() {
    brightnessStep_ = max<int>(brightnessStep_ - 1, 0);
    setBrightness();
}

void DeskLight::setMaxBrightness() {
    brightnessStep_ = pwmConfig_.steps;
    setBrightness();
}

void DeskLight::setMinBrightness() {
    brightnessStep_ = 0;
    setBrightness();
}
