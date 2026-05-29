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
      brightnessLevel_(0) {}

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
    ledcWrite(pwmPin_, brightnessLevel_);
}

void DeskLight::increaseBrightness() {
    brightnessLevel_ = min<int>(brightnessLevel_ + pwmConfig_.step, pwmConfig_.upperLimit);
    setBrightness();
}

void DeskLight::decreaseBrightness() {
    brightnessLevel_ = max<int>(brightnessLevel_ - pwmConfig_.step, pwmConfig_.lowerLimit);
    setBrightness();
}

void DeskLight::setMaxBrightness() {
    brightnessLevel_ = pwmConfig_.upperLimit;
    setBrightness();
}

void DeskLight::setMinBrightness() {
    brightnessLevel_ = pwmConfig_.lowerLimit;
    setBrightness();
}
