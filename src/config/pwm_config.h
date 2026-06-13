#pragma once

#include <stdint.h>

struct PwmConfig {
    uint32_t frequency;
    uint8_t resolution;
    uint16_t lowerLimit;
    uint16_t upperLimit;
    uint8_t steps;
    float gamma;
};
