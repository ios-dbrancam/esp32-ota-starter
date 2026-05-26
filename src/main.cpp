#include <Arduino.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include "config/config.h"
#include "config/secrets.h"
#include "ota/ota.h"

void setup() {
  Serial.begin(115200);
  setupOta(otaHost, otaPwd);
}

void loop() {
  handleOta();
}
