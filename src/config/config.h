#pragma once

// Left Brightness
const uint8_t leftBrightnessUp = 26; // D0
const uint8_t leftBrightnessDown = 27; // D1

// Right Brightness
const uint8_t rightBrightnessUp = 7; // D5
const uint8_t rightBrightnessDown = 6; // D4

// PWM Pins
const uint8_t leftPwmPin = 3; // D10
const uint8_t rightPwmPin = 4; // D9

// PWM Values
const uint8_t minPwm = 0;
const uint8_t maxPwm = 255;
const uint8_t stepPwm = 8;

// Debounce
const uint8_t inputDebounceWindow = 100;

// Static IP
IPAddress localIp(192, 168, 0, 244);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet (255, 255, 255, 0);
IPAddress dns(8, 8, 8, 8);

// mDNS
const char* hostname = "deskLights";
