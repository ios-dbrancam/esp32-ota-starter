#pragma once

#include <stdint.h>

struct MqttConfig {
    const char* broker;
    uint16_t port = 1883;
    const char* clientId;
    const char* username;
    const char* password;
    const char* topicRoot;
    const char* deviceName;
};
