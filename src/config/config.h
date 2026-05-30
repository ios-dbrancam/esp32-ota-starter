#pragma once

// Pin definitions
// const uint8_t PIN = 1;

// Constants
// const XXX XXX = XX;

// Static IP
const IPAddress localIp(192, 168, 0, 244);
const IPAddress gateway(192, 168, 0, 1);
const IPAddress subnet (255, 255, 255, 0);
const IPAddress dns(8, 8, 8, 8);

// mDNS
const char* const hostname = "{HOSTNAME}";
