#include <Arduino.h>
#include "config/config.h"
#include "config/pwm_config.h"
#include "config/secrets.h"
#include "desk_light/desk_light.h"
#include "network/network.h"
#include "ota/ota.h"

const PwmConfig pwmConfig = {
  .frequency = pwmFrequency,
  .resolution = pwmResolution,
  .lowerLimit = minPwm,
  .upperLimit = maxPwm,
  .step = stepPwm,
};

DeskLight leftLight(leftPwmPin, leftBrightnessUp, leftBrightnessDown, pwmConfig, debounceTime);
DeskLight rightLight(rightPwmPin, rightBrightnessUp, rightBrightnessDown, pwmConfig, debounceTime);

void setup() {
  Serial.begin(115200);
  setupNetwork(networkSsid, networkPwd, localIp, gateway, subnet, dns, hostname);
  setupOta(hostname, otaPassword);

  leftLight.initialize();
  rightLight.initialize();
}

void loop() {
  ensureNetwork();
  handleOta();

  leftLight.update();
  rightLight.update();
}
