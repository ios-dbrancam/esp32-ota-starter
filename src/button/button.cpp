#include "button/button.h"

Button::Button(uint8_t pin,
               unsigned long debounceTime,
               unsigned long longPressTime)
    : pin_(pin),
      debounceTime_(debounceTime),
      longPressTime_(longPressTime),
      lastRead_(LOW),
      lastStable_(LOW),
      lastChangeTime_(0),
      pressStartTime_(0),
      longPressFired_(false),
      tapEvent_(false),
      longPressEvent_(false) {}

void Button::initialize() {
    pinMode(pin_, INPUT);
}

void Button::update(unsigned long now) {
    bool reading = digitalRead(pin_);

    // Restart debounce timer on edge
    if (reading != lastRead_) {
        lastRead_ = reading;
        lastChangeTime_ = now;
    }

    // Stable state
    if ((now - lastChangeTime_) >= debounceTime_ && reading != lastStable_) {
        lastStable_ = reading;

        if (reading == HIGH) {
            pressStartTime_ = lastChangeTime_;
            longPressFired_ = false;
        } else {
            if (!longPressFired_) {
                tapEvent_ = true;
            }
        }
    }

    // While still pressed
    if ((lastStable_ == HIGH) && !longPressFired_ && (now - pressStartTime_) >= longPressTime_) {
        longPressEvent_ = true;
        longPressFired_ = true;
    }
}

bool Button::wasTapped() {
    bool fired = tapEvent_;
    tapEvent_ = false;
    return fired;
}

bool Button::wasLongPressed() {
    bool fired = longPressEvent_;
    longPressEvent_ = false;
    return fired;
}
