#pragma once
#include <IPAddress.h>

void setupNetwork(
    const char* ssid,
    const char* password,
    IPAddress localIp = IPAddress(),
    IPAddress gateway = IPAddress(),
    IPAddress subnet = IPAddress(),
    IPAddress dns = IPAddress(),
    const char* hostname = nullptr
);

void ensureNetwork();
