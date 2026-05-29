#pragma once

// Left Brightness
const uint8_t leftBrightnessUp = 2;
const uint8_t leftBrightnessDown = 3;

// Right Brightness
const uint8_t rightBrightnessUp = 7;
const uint8_t rightBrightnessDown = 6;

// PWM Pins
const uint8_t leftPwmPin = 10;
const uint8_t rightPwmPin = 9;

// PWM Values
const uint32_t pwmFrequency = 5000;  // 5kHz
const uint8_t pwmResolution = 12;    // 0 ... 4096
const uint8_t minPwm = 0;
const uint16_t maxPwm = 4095;
const uint8_t stepPwm = 128;         // 32 Steps

// Debounce
const uint8_t debounceTime = 100;

// Static IP
const IPAddress localIp(192, 168, 0, 244);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet (255, 255, 255, 0);
const IPAddress dns(8, 8, 8, 8);

// mDNS
const char* const hostname = "deskLightsController";
