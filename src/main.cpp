#include <Arduino.h>
#include "config/config.h"
#include "config/secrets.h"
#include "network/network.h"
#include "ota/ota.h"

void setup() {
  Serial.begin(115200);
  setupNetwork(networkSsid, networkPwd, localIp, gateway, subnet, dns, hostname);
  setupOta(hostname, otaPassword);
}

void loop() {
  ensureNetwork();
  handleOta();
}
