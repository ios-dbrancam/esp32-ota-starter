#pragma once

#include <IPAddress.h>

struct NetworkConfig {
    const char* ssid;
    const char* password;
    IPAddress localIp = IPAddress();
    IPAddress gateway = IPAddress();
    IPAddress subnet = IPAddress();
    IPAddress dns = IPAddress();
    const char* hostname = nullptr;
    unsigned long connectTimeout = 15 * 1000;
};
