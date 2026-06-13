#include "config/pwm_config.h"
#include "desk_light/desk_light.h"
#include "logger/logger.h"

DeskLight::DeskLight(const char* name,
                     uint8_t pwmPin,
                     uint8_t upButtonPin,
                     uint8_t downButtonPin,
                     PwmConfig pwmConfig,
                     unsigned long debounceTime,
                     unsigned long longPressTime)
    : name_(name),
      pwmPin_(pwmPin),
      upButton_(upButtonPin, debounceTime, longPressTime),
      downButton_(downButtonPin, debounceTime, longPressTime),
      pwmConfig_(pwmConfig),
      brightnessStep_(0),
      currentPwm_(0),
      initialPwm_(0),
      targetPwm_(0),
      fadeStartTime_(0),
      fadeDuration_(300) {}

void DeskLight::initialize() {
    ledcAttach(pwmPin_, pwmConfig_.frequency, pwmConfig_.resolution);
    ledcWrite(pwmPin_, 0);
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

    processFade(now);
}

void DeskLight::setBrightness() {
    // Gamma correction (Logarithmic)
    // OUT = (IN / MaxIN)^Gamma * MaxOUT
    float normalized = (float)brightnessStep_ / (float)pwmConfig_.steps;
    float corrected = pow(normalized, pwmConfig_.gamma);
    targetPwm_ = (uint32_t)(corrected * pwmConfig_.upperLimit);
    initialPwm_ = currentPwm_;
    fadeStartTime_ = millis();
}

void DeskLight::processFade(unsigned long now) {
    if (currentPwm_ == targetPwm_) return;

    unsigned long elapsedTime = now - fadeStartTime_;

    if (elapsedTime > fadeDuration_) {
        currentPwm_ = targetPwm_;
    } else {
        int32_t deltaPwm = (int32_t)targetPwm_ - (int32_t)initialPwm_;
        float normalizedTime = (float)elapsedTime / (float)fadeDuration_;
        float easedTime = normalizedTime * normalizedTime * (3.0f - (2.0f * normalizedTime));
        currentPwm_ = (uint32_t)((int32_t)initialPwm_ + (int32_t)(deltaPwm * easedTime));
    }

    ledcWrite(pwmPin_, currentPwm_);
}

void DeskLight::increaseBrightness() {
    brightnessStep_ = min<int>(brightnessStep_ + 1, pwmConfig_.steps);
    setBrightness();
    logger.log("Desk light ID: " + String(name_) + " increased to " + String(brightnessStep_));
}

void DeskLight::decreaseBrightness() {
    brightnessStep_ = max<int>(brightnessStep_ - 1, 0);
    setBrightness();
    logger.log("Desk light ID: " + String(name_) + " decreased to " + String(brightnessStep_));
}

void DeskLight::setMaxBrightness() {
    brightnessStep_ = pwmConfig_.steps;
    setBrightness();
    logger.log("Desk light ID: " + String(name_) + " set to " + String(brightnessStep_));
}

void DeskLight::setMinBrightness() {
    brightnessStep_ = 0;
    setBrightness();
    logger.log("Desk light ID: " + String(name_) + " set to " + String(brightnessStep_));
}
