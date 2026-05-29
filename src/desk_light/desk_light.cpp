#include "desk_light/desk_light.h"
#include "config/pwm_config.h"

DeskLight::DeskLight(uint8_t pwmPin, uint8_t upButton, uint8_t downButton, PwmConfig pwmConfig, uint16_t debounceTime)
    : pwmPin_(pwmPin),
      upButton_(upButton),
      downButton_(downButton),
      pwmConfig_(pwmConfig),
      debounceTime_(debounceTime),
      brightnessLevel_(0) {}

void DeskLight::initialize() {
    ledcAttach(pwmPin_, pwmConfig_.frequency, pwmConfig_.resolution);
    setBrightness();
    upButton_.initialize();
    downButton_.initialize();
}

void DeskLight::update() {
    unsigned long now = millis();

    if (upButton_.justPressed(now, debounceTime_)) {
        increaseBrightness();
    }

    if (downButton_.justPressed(now, debounceTime_)) {
        decreaseBrightness();
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
