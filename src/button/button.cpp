#include "button/button.h"

Button::Button(uint8_t pin)
    : pin_(pin), lastRead_(LOW), lastStable_(LOW), lastChangeTime_(0) {}

void Button::initialize() {
    pinMode(pin_, INPUT);
}

bool Button::justPressed(unsigned long now, uint16_t debounceTime) {
    bool reading = digitalRead(pin_);

    if (reading != lastRead_) {
        lastRead_ = reading;
        lastChangeTime_ = now;
    }

    if ((now - lastChangeTime_) >= debounceTime && reading != lastStable_) {
        lastStable_ = reading;
        if (reading == HIGH) {
            return true;
        }
    }
    return false;
}
