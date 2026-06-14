#include "config/pwm_config.h"
#include "desk_light/desk_light.h"
#include "logger/logger.h"

DeskLight::DeskLight(uint8_t pwmPin,
                     const char* id,
                     uint8_t upButtonPin,
                     uint8_t downButtonPin,
                     PwmConfig pwmConfig,
                     unsigned long debounceTime,
                     unsigned long longPressTime,
                     uint8_t defaultOnStep)
    : pwmPin_(pwmPin),
      id_(id),
      upButton_(upButtonPin, debounceTime, longPressTime),
      downButton_(downButtonPin, debounceTime, longPressTime),
      pwmConfig_(pwmConfig),
      defaultOnStep_(defaultOnStep),
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

bool DeskLight::isOn() const {
    return brightnessStep_ > 0;
}

const char* DeskLight::getId() const {
    return id_;
}

void DeskLight::setBrightnessStep(int step) {
    brightnessStep_ = min<int>(max<int>(step, 0), pwmConfig_.steps);
    setBrightness();
}

void DeskLight::turnOn() {
    if (isOn()) return;
    setBrightnessStep(defaultOnStep_);
    logger.log("Desk light ID: " + String(id_) + " turned ON via MQTT");
}

void DeskLight::turnOff() {
    setBrightnessStep(0);
    logger.log("Desk light ID: " + String(id_) + " turned OFF via MQTT");
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
    setBrightnessStep(brightnessStep_ + 1);
    logger.log("Desk light ID: " + String(id_) + " increased to " + String(brightnessStep_));
}

void DeskLight::decreaseBrightness() {
    setBrightnessStep(brightnessStep_ - 1);
    logger.log("Desk light ID: " + String(id_) + " decreased to " + String(brightnessStep_));
}

void DeskLight::setMaxBrightness() {
    setBrightnessStep(pwmConfig_.steps);
    logger.log("Desk light ID: " + String(id_) + " set to " + String(brightnessStep_));
}

void DeskLight::setMinBrightness() {
    setBrightnessStep(0);
    logger.log("Desk light ID: " + String(id_) + " set to " + String(brightnessStep_));
}
