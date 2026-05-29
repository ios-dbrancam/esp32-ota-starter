#pragma once
#include <Arduino.h>

class Button {
    public:
        explicit Button(uint8_t pin);

        void initialize();
        bool justPressed(unsigned long now, uint16_t debounceTime);
    
    private:
        uint8_t pin_;
        bool lastRead_;
        bool lastStable_;
        unsigned long lastChangeTime_;
};
