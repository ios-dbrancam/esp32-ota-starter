#pragma once

#include <stdint.h>

struct LoggerConfig {
    uint16_t port = 23;
    bool serialMirror = true;
};
