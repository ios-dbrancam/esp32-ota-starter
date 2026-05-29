#pragma once
#include <Arduino.h>

class Button {
    public:
        explicit Button(uint8_t pin,
                        unsigned long debounceTime,
                        unsigned long longPressTime);

        void initialize();
        void update(unsigned long now);

        bool wasTapped();
        bool wasLongPressed();
    
    private:
        uint8_t pin_;
        unsigned long debounceTime_;
        unsigned long longPressTime_;

        bool lastRead_;
        bool lastStable_;
        unsigned long lastChangeTime_;

        unsigned long pressStartTime_;
        bool longPressFired_;

        bool tapEvent_;
        bool longPressEvent_;
};
